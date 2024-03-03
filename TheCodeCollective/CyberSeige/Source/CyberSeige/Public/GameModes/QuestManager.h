// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStepComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextStep);

USTRUCT(BlueprintType)
struct FQuestStep
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnStepComplete OnStepComplete;

	// The text that is displayed in the quest step list widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText QuestStepText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UActorComponent*> Triggers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<AActor*> Activators;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bInitialStep;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bFinalStep;

	// Determines if the quest manager should halt the quest while looking for triggers for this step.
	// This option is mainly for triggers that will be spawned some time after the quest starts.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSkipIfNoTriggers;

	// Determines if a step uses multiple triggers that all need to be hit
	// Does not apply for a step with multiple triggers where only one needs to be hti
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsMultiTrigger;

	// Determines if the step needs a quest indicator arrow or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bUsesIndicator;

	// The text that is displayed in the dialogue box
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString DialogueText;

	// Keeps track of the current triggers bound
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int BoundTriggerCount;

	// Used in multi-trigger steps to determine the amount of triggers needed to be bound
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int TriggersNeeded;

	// Used in multi-trigger steps to keep track of the triggers hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int TriggersHit;

	// Automatically Assigned
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int StepNumber;

	// Image for the dialogue box
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* DialogueBoxImage;

	bool AddTrigger(UActorComponent* NewTrigger)
	{
		if (NewTrigger != nullptr)
		{
			Triggers.Add(NewTrigger);
			return true;
		}
		else return false;
	}

	bool AddActivator(AActor* NewActivator)
	{
		if (NewActivator != nullptr)
		{
			Activators.Add(NewActivator);
			return true;
		}
		else return false;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveStepChanged, FQuestStep, NewStep);

USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()

	// Key used for finding the quest in the quest library
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString QuestKey;

	// Final dialogue that is displayed after completing a quest
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FText QuestCompleteMessage;

	// Broadcasts to the manager when the quest is complete
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnQuestComplete OnQuestComplete;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnActiveStepChanged OnActiveStepChanged;

	// Holds all of our quest steps in order
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FQuestStep> QuestSteps;

	// Keeps track of our active step
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FQuestStep ActiveStep;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestBegin);

UCLASS()
class CYBERSEIGE_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestManager();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FQuest ActiveQuest;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString QuestCompleteMessage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bQuestSetupComplete;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnNextStep OnNextStep;


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnQuestBegin OnQuestBegin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnQuestComplete OnQuestEnded;

	// These two functions would ideally be in the quest and step structs, but
	// my brain is not big enough to figure out how to do that and still be able
	// to bind them to event delegates
	UFUNCTION(BlueprintCallable)
		void NextStep();

	UFUNCTION(BlueprintCallable)
		void CompleteStep(USceneComponent* Trigger);

	UFUNCTION(BlueprintCallable)
		void EnableTriggers(FQuestStep& QuestStep);
	
	UFUNCTION(BlueprintCallable)
		void DisableTriggers(FQuestStep& QuestStep);

	UFUNCTION(BlueprintCallable)
		void StartTimers();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "SetupQuest"))
		void ReceiveSetupQuest();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName = "QuestFinished"))
		void ReceiveQuestFinished();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "SetHudMessage"))
		void ReceiveSetHudMessage();

	UFUNCTION(BlueprintCallable)
		bool GetStep(int StepNumber, struct FQuestStep& OutStruct);

	UFUNCTION(BlueprintCallable)
		FQuestStep BindStep(FQuestStep Step);

	UFUNCTION(BlueprintCallable)
		void BindQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This will loop every 1 second and check the quest steps to make sure they all have triggers
	UFUNCTION(BlueprintCallable)
		void WaitForStepTriggers();

	// Called every 1 second. Finds new activators in the world
	UFUNCTION(BlueprintCallable)
		void GetActivators();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int StepIndex;

	UPROPERTY()
		FTimerHandle WaitHandle;
};
