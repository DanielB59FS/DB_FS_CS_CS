// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/EnemyInterface.h"
#include "WaveGameMode.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateVariable, FText, Message);

UCLASS()
class CYBERSEIGE_API AWaveGameMode : public AGameModeBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	AWaveGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Dispatcher")
		FDelegateVariable OnGameEnded;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyClasses")
		TSubclassOf<class ABaseEnemy> EnemyClass1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyClasses")
		TSubclassOf<class ABaseEnemy> EnemyClass2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyClasses")
		TSubclassOf<class ABaseEnemy> EnemyClass3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyClasses")
		TSubclassOf<class ABaseEnemy> EnemyClass4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemyClasses")
		TSubclassOf<class ABaseEnemy> EnemyClass5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LoopVariables")
		int32 IndividualLoopVariable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LoopVariables")
		int32 SpawnerLoopVariable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LoopVariables")
		int32 SpawnerAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 EnemyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 WaveCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 CountdownToDefensePhase = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 CountdownToDefensePhaseReset = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool DefensePhase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UEditableText* CountDownTimer;

	UFUNCTION(BlueprintCallable)
		void EnemyAdded();

	UFUNCTION(BlueprintCallable)
		void EnemyDied();

	UFUNCTION(BlueprintCallable)
		void CheckEnemyCount();

	UFUNCTION(BlueprintCallable)
		void FortifyPhase();

	UFUNCTION(BlueprintCallable)
		void Spawners();

	UFUNCTION(BlueprintCallable)
		void SetCountdownTimer();

	UFUNCTION(BlueprintCallable)
		void CastToCorrectSpawner(int32 SpawnerVariable, AActor* Spawner);

	// start IEnemyInterface

	virtual void AllEnemiesDead() override;

	virtual void Attack(UObject* ControlledPawn) override;

	// end IEnemyInterface
};
