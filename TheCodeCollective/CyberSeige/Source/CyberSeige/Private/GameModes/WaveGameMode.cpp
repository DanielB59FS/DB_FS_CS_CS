// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WaveGameMode.h"
#include "Components/EditableText.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Spawner.h"

AWaveGameMode::AWaveGameMode()
{

}

void AWaveGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AWaveGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DefensePhase)
		FortifyPhase();
}

void AWaveGameMode::EnemyAdded()
{
	EnemyCount++;
}

void AWaveGameMode::EnemyDied()
{
	EnemyCount--;
	CheckEnemyCount();
}

void AWaveGameMode::CheckEnemyCount()
{
	if (EnemyCount <= 0)
	{
		AllEnemiesDead();

		DefensePhase = false;
		
		if (WaveCount == 100)
			OnGameEnded.Broadcast(FText::FromString("You Win!"));
	}
}

void AWaveGameMode::FortifyPhase()
{
	if (CountdownToDefensePhase != 0)
	{
		if(nullptr == CountDownTimer)
		{
			SetCountdownTimer();
			CountDownTimer->SetVisibility(ESlateVisibility::Visible);
			UKismetSystemLibrary::Delay(GetWorld(), 1.0f, FLatentActionInfo());
			CountdownToDefensePhase--;
			FortifyPhase();
		}
		else
		{
			CountDownTimer->SetVisibility(ESlateVisibility::Visible);
			UKismetSystemLibrary::Delay(GetWorld(), 1.0f, FLatentActionInfo());
			CountdownToDefensePhase--;
			FortifyPhase();
		}
	}
	else
	{
		DefensePhase = true;
		CountdownToDefensePhase = CountdownToDefensePhaseReset;                               
		WaveCount++;
		CountDownTimer->SetVisibility(ESlateVisibility::Hidden);
		SpawnerLoopVariable = 0;
		IndividualLoopVariable = 1;
		Spawners();
	}
}

void AWaveGameMode::Spawners()
{
	TArray <class AActor*> Spawners;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Spawner"), Spawners);
	SpawnerAmount = Spawners.Num();
	int32 amountToSpawn = FMath::TruncToInt(WaveCount * FMath::FRandRange(1.01f, 1.99f) * 10);

	for (SpawnerLoopVariable; SpawnerLoopVariable < SpawnerAmount; ++SpawnerLoopVariable)
	{
		for (IndividualLoopVariable; IndividualLoopVariable <= amountToSpawn; ++IndividualLoopVariable)
		{
			CastToCorrectSpawner(SpawnerLoopVariable, Spawners[SpawnerLoopVariable]);
			UKismetSystemLibrary::Delay(GetWorld(), .25f, FLatentActionInfo());
		}

		IndividualLoopVariable = 1;
	}
}

void AWaveGameMode::SetCountdownTimer()
{

}

void AWaveGameMode::CastToCorrectSpawner(int32 SpawnerVariable, AActor* Spawner)
{
	ASpawner* spawner = Cast<ASpawner>(Spawner);
	if (spawner)
		spawner->AddEnemyToQueue(EnemyClass1);
}

void AWaveGameMode::AllEnemiesDead()
{

}

void AWaveGameMode::Attack(UObject* ControlledPawn)
{

}