// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spawner.h"
#include "Components/SphereComponent.h"
#include "Actors/BaseEnemy.h"
#include "Engine/World.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASpawner::SpawnEnemy()
{
	if (EnemyQueue[0])
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform transform = GetActorTransform();

		ABaseEnemy* newEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyQueue[0], transform, SpawnParams);

		EnemyQueue.RemoveAt(0);

		if (newEnemy)
			return true;
		else
			return false;
	}
	else
		return false;
}

void ASpawner::AddEnemyToQueue(TSubclassOf<class ABaseEnemy> classOfEnemy)
{
	EnemyQueue.Add(classOfEnemy);
	SpawnEnemy();
}
