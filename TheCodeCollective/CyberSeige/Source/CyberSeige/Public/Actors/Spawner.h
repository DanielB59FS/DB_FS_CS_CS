// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class CYBERSEIGE_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ABaseEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray <TSubclassOf<class ABaseEnemy>> EnemyQueue;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void AddEnemyToQueue(TSubclassOf<class ABaseEnemy> ClassOfEnemy);

private:
	UFUNCTION()
		bool SpawnEnemy();
};
