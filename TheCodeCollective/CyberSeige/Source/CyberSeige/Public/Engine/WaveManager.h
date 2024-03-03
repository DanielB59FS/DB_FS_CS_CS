// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaveManager.generated.h"

// Struct used for keeping track of enemy spawn chances
USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_BODY()

	// A reference to the enemy class.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString DebugName;

	// A reference to the enemy class.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ACharacter> EnemyClass;

	// The lowest chance an enemy has to spawn. Doesn't change
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float BaseSpawnProbability;

	// The curren chance an enemy has to spawn. Changes
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float CurrentSpawnProbability;

	// The highest chance an enemy has to sapwn. Doesn't change
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float MaxSpawnProbability;

	// The wave that the enemy becomes available to spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int SpawnUnlockWave;

	// Determines when the enemy is most likely to spawn.
	// Enemy spawn chance may decrease after this wave
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int SpawnProbabilityPeakWave;

	// If the enemy's spawn chance decreases afte the peak wave, this wave
	// determines how fast it goes back to base
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int SpawnProbabilityLowWave;

	// Determines if the enemy's spawn chance decreases after their peak wave
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bChanceDecreaseAfterPeak;

	// Increases probability based on the below formula
	// Formula: Current += (Max - Current) / (PeakWave - CurWave)
	// Current = Clamp(Base, Max, Current)
	void IncreaseProbability(float CurWave)
	{
		CurrentSpawnProbability += (MaxSpawnProbability - CurrentSpawnProbability) / (SpawnProbabilityPeakWave - CurWave);
		CurrentSpawnProbability = FMath::Clamp(MaxSpawnProbability, BaseSpawnProbability, CurrentSpawnProbability);
	}

	// Decreases probability based on the below formula
	// Formula: Current -= (Current - Base) / ((LowWave + 1) - CurWave)
	// Current = Clamp(Base, Max, Current)
	void DecreaseProbability(float CurWave)
	{
		CurrentSpawnProbability -= (CurrentSpawnProbability - BaseSpawnProbability) / ((SpawnProbabilityLowWave + 1) - CurWave);
		CurrentSpawnProbability = FMath::Clamp(MaxSpawnProbability, BaseSpawnProbability, CurrentSpawnProbability);
	}
};


// Struct used for creating premade waves
USTRUCT(BlueprintType)
struct FPremadeWave
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		TArray<TSubclassOf<class ACharacter>> PremadeSpawnList;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveBegin);

UCLASS()
class CYBERSEIGE_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveManager();

	// Event delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Wave Manager | Event Delegates")
		FOnWaveEnded OnWaveEnded;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Wave Manager | Event Delegates")
		FOnWaveBegin OnWaveBegin;

	// Starts the series of functions that must be run to start a wave
	UFUNCTION(BlueprintCallable, Category = "Wave Manager | Functions")
		void StartWaveProcess(int CurWave);

	// Spawner locations 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		TArray<AActor*> Spawners;

	// Enemy spawn data (Spawn Probabilities, class references, other configurable data)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		TArray<FEnemySpawnData> EnemySpawnData;

	// Used for creating premade waves, potentially for tutorials or set points in the game
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		TArray<FPremadeWave> PremadeWaves;

	// Actual list of enemies that will be spawned in the wave
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		TArray<TSubclassOf<class ACharacter>> SpawnList;

	// Used for modifying stats based on difficulty
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Manager | Variables")
		float DifficultyModifier;

	// Determines the increase of enemies spawned each round
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Wave Manager | Variables")
		int EnemyIncreaseModifier;

	// Keeps track of the current wave
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Wave Manager | Variables")
		int CurrentWave;

	// Keeps track of the final wave. Used with CurrentWave to calculate probabilities
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Wave Manager | Variables")
		int FinalWave;

	// Determines if the manager will be using premade waves
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Wave Manager | Variables")
		bool bSpawnPremadeWaves;

	// Adjusts the spawn probabilities for all enemies
	UFUNCTION(BlueprintCallable,  Category = "Wave Manager | Functions")
		void CalculateProbabilities();

	// Calculates the number of enemies based on what wave we're on
	UFUNCTION(BlueprintCallable, Category = "Wave Manager | Functions")
		void CalculateNumberOfEnemies();

	// Uses enemy spawn probability to populate the spawn array with enemy class references
	UFUNCTION(BlueprintCallable, Category = "Wave Manager | Functions")
		void PopulateSpawnList();

	// Sets a looping timer that will start spawning enemies
	UFUNCTION(BlueprintCallable, Category = "Wave Manager | Functions")
		void StartSpawning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		int EnemiesToSpawn;

	UPROPERTY()
		int NumberOfSpawners;

	UPROPERTY()
		float SpawnDelay;

	UPROPERTY()
		FTimerHandle SpawnDelayTimer;

	// Runs on a timer
	// Will spawn the next enemy and decrease the amount of enemies to spawn.
	// Once the amount of enemies to spawn reaches zero, it clears and invalidates the timer
	UFUNCTION()
		void SpawnNext();

	// Cleans up the SpawnList in case any enemies are left over
	UFUNCTION()
		void CleanupSpawnLists();
};
