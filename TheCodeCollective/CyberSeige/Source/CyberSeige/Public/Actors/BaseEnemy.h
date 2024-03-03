// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseCharacter.h"
#include "Interfaces/EnemyInterface.h"

#include "BaseEnemy.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateVariableNoParams);

UCLASS()
class CYBERSEIGE_API ABaseEnemy : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Dispatcher")
		FDelegateVariableNoParams OnEnemySpawned;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Dispatcher")
		FDelegateVariableNoParams OnEnemyDied;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* LeftHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* RightHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPawnSensingComponent* PawnSenseComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BBKeys")
		FName BBTurretKeyName = "Turret";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BBKeys")
		FName BBCastleKeyName = "Castle";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BBKeys")
		FName BBPlayerKeyName = "Player";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBlackboardComponent* Blackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float BaseDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanSeePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanAttack;

	UPROPERTY(BlueprintReadWrite)
		class UUserWidget* PauseMenu;

	UPROPERTY(BlueprintReadWrite)
		class ACastle* Castle;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Die(float healthRatio);

	UFUNCTION()
		void HandleSeenPawn(APawn* Pawn);

	UFUNCTION()
		void HandleComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void HandlePerceptionUpdated(AActor* Pawn, int Stimulus);

	// start IEnemyInterface

	virtual void AllEnemiesDead() override;

	virtual void Attack(UObject* ControlledPawn) override;

	// end IEnemyInterface
};
