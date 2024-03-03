// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Interfaces/QuestActivatorInterface.h"
#include "Enums\Enums.h"
#include "QuestActivatorComponent.generated.h"

#define MAX_ACTIVATOR_FUNCTIONS 10

#define ROTATE_OBJECT_X 0
#define ROTATE_OBJECT_Y 1
#define ROTATE_OBJECT_Z 2
#define START_WAVE_TIMER 3
#define ENABLE_SNIPER_TURRET 4
#define ENABLE_LASER_FENCE 5
#define ENABLE_TELEPORTER 6
#define GIVE_SCRAP 7
#define DAMAGE_TURRETS 8
#define DESTROY_ACTORS 9

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartWaveTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTurrets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnableMenuItem, EDefenseType, Type, int, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGiveScrap, int, ScrapToGive);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERSEIGE_API UQuestActivatorComponent : public USceneComponent, public IQuestActivatorInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestActivatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int BoundStep;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString BoundQuestKey;

	// Specifically used for starting the wave timer.
	// This event is bound in the quest manager
	// This has to be done since the game mode is still in a blueprint
	UPROPERTY(BlueprintCallable, BlueprintAssignable, EditAnywhere)
		FOnStartWaveTimer OnStartWaveTimer;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, EditAnywhere)
		FOnDamageTurrets OnDamageTurrets;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, EditAnywhere)
		FOnEnableMenuItem OnEnableMenuItem;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, EditAnywhere)
		FOnGiveScrap OnGiveScrap;

	// typedef for function pointers
	typedef void (UQuestActivatorComponent::* FunctionPtrType)();

	// Cariable for determining the activate function
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int BehaviorIndex;

	// Variables for the activation functions
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DegreesToRotate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float RotateSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int ScrapToGive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<AActor> ActorsToDestroy;


	// Function pointer array so we can choose a behavior in game
	FunctionPtrType ActivateFunctions[MAX_ACTIVATOR_FUNCTIONS];
		
	// Begin QuestActivatorInterface
	virtual void StartActivate_Implementation() override;

	virtual int GetBoundStep_Implementation() override;
	// End QuestActivatorInterface

private:
	UPROPERTY()
		bool bRotate;

	UPROPERTY(VisibleAnywhere)
		FRotator TargetRotation;

	// These functions are the different ways the quest can "Activate" something
	UFUNCTION()
		void RotateObjectX();

	UFUNCTION()
		void RotateObjectY();

	UFUNCTION()
		void RotateObjectZ();

	UFUNCTION()
		void RotateObject(float Amount);

	UFUNCTION()
		void StartWaveTimer();

	UFUNCTION()
		void EnableSniperTurret();
	
	UFUNCTION()
		void EnableLaserFence();

	UFUNCTION()
		void EnableTeleporter();

	UFUNCTION()
		void GiveScrap();

	UFUNCTION()
		void DamageTurrets();

	UFUNCTION()
		void DestroySpecifiedActors();
};
