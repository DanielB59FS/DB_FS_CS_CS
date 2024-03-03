// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "QuestTriggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompleteStep);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERSEIGE_API UQuestTriggerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestTriggerComponent();

	UFUNCTION(BlueprintCallable)
		void CompleteStep();

	UPROPERTY(BlueprintCallable)
		FOnCompleteStep OnCompleteStep;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bTriggerActive;

	// Used for dynamically binding triggers that are only available during runtime
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int BoundStep;

	// Used to ensure that the step is bound to the correct quest
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString BoundQuestKey;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Loops repeatedly until it finds the quest manager
	UFUNCTION()
		void WaitForQuestManager();

	UPROPERTY()
		FTimerHandle WaitTimer;
};
