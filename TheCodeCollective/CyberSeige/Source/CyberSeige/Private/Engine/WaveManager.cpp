// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/WaveManager.h"
#include "../../CyberSeige.h"
#include "GameFramework/Character.h"

// Sets default values
AWaveManager::AWaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DifficultyModifier = 1.0f;
	EnemyIncreaseModifier = 10.0f;
	SpawnDelay = 0.25f;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Turn off this bool value if there are no premade waves to spawn
	//
}

// Starts the series of functions that must be run to start a wave
void AWaveManager::StartWaveProcess(int CurWave)
{
	OnWaveBegin.Broadcast();
	CurrentWave = CurWave;

	if (bSpawnPremadeWaves && PremadeWaves.Num() <= 0) bSpawnPremadeWaves = false;
	else if (bSpawnPremadeWaves && PremadeWaves.Num() > 0)
	{
		if (PremadeWaves.Num() <= 0) return;

		SpawnList = PremadeWaves[0].PremadeSpawnList;
		StartSpawning();
	}
	else
	{
		CalculateProbabilities();
		CalculateNumberOfEnemies();
		PopulateSpawnList();
		StartSpawning();
	}
}

// Adjusts the spawn probabilities for all enemies
void AWaveManager::CalculateProbabilities()
{
	UE_LOG(Game, Error, TEXT("Start of wave %d"), CurrentWave)
	for (FEnemySpawnData &Data : EnemySpawnData)
	{
		UE_LOG(Game, Warning, TEXT("%s probability is %f"), *Data.DebugName, Data.CurrentSpawnProbability);

		// Skip if enemy is locked 
		if (Data.SpawnUnlockWave > CurrentWave) continue;

		// Check if we have hit the peak wave for the enemy
		// If we are at or below the peak, increase
		if (Data.SpawnProbabilityPeakWave >= CurrentWave)
		{
			Data.IncreaseProbability(CurrentWave);
			UE_LOG(Game, Warning, TEXT("%s probability increased to %f"), *Data.DebugName, Data.CurrentSpawnProbability);
		}
		// Otherwise, if it is supposed to decrease after peak, decrease
		else if (Data.bChanceDecreaseAfterPeak && Data.SpawnProbabilityPeakWave < CurrentWave && Data.SpawnProbabilityLowWave >= CurrentWave)
		{
			Data.DecreaseProbability(CurrentWave);
			UE_LOG(Game, Warning, TEXT("%s probability decreased to %f"), *Data.DebugName, Data.CurrentSpawnProbability);
		}		
	}
}

// Calculates the number of enemies based on what wave we're on
void AWaveManager::CalculateNumberOfEnemies()
{
	EnemiesToSpawn = CurrentWave * EnemyIncreaseModifier * DifficultyModifier  * FMath::FRandRange(0.85f, 1.15f);

	UE_LOG(Game, Warning, TEXT("Calculated %d enemies to spawn"), EnemiesToSpawn);
}

// Populates the spawn list with all of the enemies that will be spawned this wave
void AWaveManager::PopulateSpawnList()
{
	// Create tiered list of potential spawns (0 - 4 is all of the base enemies)
	// This is very much not ideal and needs to be changed later
	float FirstIndexProbability = EnemySpawnData[0].CurrentSpawnProbability * 5;
	float SecondIndexProbability = FirstIndexProbability + EnemySpawnData[5].CurrentSpawnProbability;
	float ThirdIndexProbability = SecondIndexProbability + EnemySpawnData[6].CurrentSpawnProbability;
	float FourthIndexProbability = ThirdIndexProbability + EnemySpawnData[7].CurrentSpawnProbability;
	UE_LOG(Game, Warning, TEXT("Probability Brackets: 0 - %f, %f - %f, %f - %f, %f - %f, %f - 100.0"), FirstIndexProbability, FirstIndexProbability + 1, SecondIndexProbability, SecondIndexProbability + 1, ThirdIndexProbability, ThirdIndexProbability + 1, FourthIndexProbability, FourthIndexProbability + 1)

	int FirstIndexDebugCount = 0;
	int SecondIndexDebugCount = 0;
	int ThirdIndexDebugCount = 0;
	int FourthIndexDebugCount = 0;

	for (int i = 0; i < EnemiesToSpawn; i++)
	{
		// Calculate a random number between 0 and 100
		float RolledChance = FMath::FRand() * 100.0f;

		UE_LOG(Game, Warning, TEXT("Rolled %f"), RolledChance)

		// First section is spawning base enemies, but there are 5 of them, so we pick one at random
		if (RolledChance <= FirstIndexProbability && EnemySpawnData[0].SpawnUnlockWave <= CurrentWave)
		{
			int BaseEnemyIndex = FMath::RandRange(0, 4);
			SpawnList.Add(EnemySpawnData[BaseEnemyIndex].EnemyClass);
			FirstIndexDebugCount++;
		}
		else if (RolledChance <= SecondIndexProbability && RolledChance > FirstIndexProbability && EnemySpawnData[5].SpawnUnlockWave <= CurrentWave)
		{
			SpawnList.Add(EnemySpawnData[5].EnemyClass);
			SecondIndexDebugCount++;
		}
		else if (RolledChance <= ThirdIndexProbability && RolledChance > SecondIndexProbability && EnemySpawnData[6].SpawnUnlockWave <= CurrentWave)
		{
			SpawnList.Add(EnemySpawnData[6].EnemyClass);
			ThirdIndexDebugCount++;
		}
		else if (RolledChance <= FourthIndexProbability && RolledChance > ThirdIndexProbability && EnemySpawnData[7].SpawnUnlockWave <= CurrentWave)
		{
			SpawnList.Add(EnemySpawnData[7].EnemyClass);
			FourthIndexDebugCount++;
		}
		// Default to spawning a base enemy
		else
		{
			int BaseEnemyIndex = FMath::RandRange(0, 4);
			SpawnList.Add(EnemySpawnData[BaseEnemyIndex].EnemyClass);
			FirstIndexDebugCount++;
		}
	}

	UE_LOG(Game, Warning, TEXT("Added %d of index 0-4, %d of index 5, %d of index 6, and %d of index 7"), FirstIndexDebugCount, SecondIndexDebugCount, ThirdIndexDebugCount, FourthIndexDebugCount)
}

// Sets a looping timer that will start spawning enemies
void AWaveManager::StartSpawning()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimer, this, &AWaveManager::SpawnNext, SpawnDelay, true, 0.f);
}

// Runs on a timer
// Will spawn the next enemy and decrease the amount of enemies to spawn.
// Once the amount of enemies to spawn reaches zero, it clears and invalidates the timer
void AWaveManager::SpawnNext()
{

	// If no spawners have been set, stop trying to spawn anything
	if (Spawners.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnDelayTimer);
		OnWaveEnded.Broadcast();
		CleanupSpawnLists();
		return;
	}

	// Select our spawner and get its location
	int SpawnerIndex = FMath::RandRange(0, Spawners.Num() - 1);
	FTransform SpawnLocation = Spawners[SpawnerIndex]->GetActorTransform();

	// Spawn first enemy at selected spawner
	GetWorld()->SpawnActor<ACharacter>(SpawnList[0], SpawnLocation);

	// Remove enemy from array and decrement enemies to spawn
	SpawnList.RemoveAt(0, 1, true);
	EnemiesToSpawn--;

	// If our SpawnList is empty or we reached 0 EnemiesToSpawn, stop the timer
	if (SpawnList.Num() <= 0 || EnemiesToSpawn <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnDelayTimer);
		CleanupSpawnLists();
	}
}

// Cleans up the SpawnList in case any enemies are left over
void AWaveManager::CleanupSpawnLists()
{
	// Remove and shrink the spawnlist and set the enemies to spawn to 0		
	if (PremadeWaves.Num() >= 1)
		PremadeWaves.RemoveAt(0, 1, true);

	SpawnList.Empty();
	SpawnList.Shrink();
	EnemiesToSpawn = 0;

}

//
// Enemy Spawn Data struct:
// Enemy Class						Class reference
// Base Spawn Probability			Float
// Current Spawn Probability 		Float
// Max Spawn Probability			Float
// Unlock Wave						Int
// Peak Wave						Int
// Should decrease after peak wave 	Bool
//
// Probability increase Calculations Leading up to peak wave :
//
// Formula: Current += (Max - Current) / (PeakWave - CurWave)
//			Current = Clamp(Base, Max, Current)
//
// Example (Number represents spawn chance for that wave):
// Base = 10, Current = 10, Max = 20, CurWave = 1, PeakWave = 5, LowWave = 10
// 
// Wave 1: 10
// Wave 2 : 10 += (20 - 10) / (5 - 1) -> 10 + 2.5
// Wave 3 : 12.5 += (20 - 12.5) / (5 - 2) -> 12.5 += (7.5 / 3) = 15
// Wave 4 : 15 += (20 - 15) / (5 - 3) -> 15 += 5 / 2 = 17.5
// Wave 5 : 17.5 += (20 - 17.5) / (5 - 4) -> 2.5 / 1 = 20
//
// Probability decrease Calculations After Peak Wave (if probability is set to decrease) :
//
// Formula: Current -= (Current - Base) / ((LowWave + 1) - CurWave)
//			Current = Clamp(Base, Max, Current)
//
// Example (Number represents spawn chance for that wave):
// Base = 50, Current = 77.5, Max = 77.5, PeakWave = 4, Curwave = 5, LowWave = 8
//
// Wave 5: 77.5 -= (77.5 - 50) / (9 - 5) -> 77.5 -= 27.5 / 4 = 70.625
// wave 6 : 70.625 -= (70.625 - 50) / (9 - 6) -> 20.625 / 3 = 63.75
// wave 7 : 63.75 -= (63.75 - 50) / (9 - 7) -> 13.75 / 2 = 56.875
// Wave 8 : 56.875 -= (56.875 - 50) / 9 - 8) -> 6.875 / 1 = 50