// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/QuestManager.h"
#include "../../CyberSeige.h"
#include "Components/QuestTriggerComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Interfaces/QuestActivatorInterface.h"
#include "Components/QuestActivatorComponent.h"
#include "Interfaces/QuestTriggerInterface.h"
#include "Engine/Texture2D.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called by an object that has set the ActiveQuest for the quest manager
void AQuestManager::StartTimers()
{
	// We do not have a valid quest to start 
	if (ActiveQuest.QuestSteps.Num() == 0 || ActiveQuest.QuestKey.Compare("NULL") == 0) return;

	// Start our loop waiting for all steps to have triggers
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, this, &AQuestManager::WaitForStepTriggers, 1, true, 0);

	FTimerHandle TempHandle;

	// Start our loop waiting for all steps to have triggers
	GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AQuestManager::GetActivators, 1, true, 0);
}

// Loops repeatedly until we have triggers
void AQuestManager::WaitForStepTriggers()
{
	bool bAllTriggersBound = true;

	int StepCount = 0;

	for (FQuestStep& Step : ActiveQuest.QuestSteps)
	{
		Step.StepNumber = StepCount;

		// Break if no triggers are assigned
		if (Step.Triggers.Num() <= 0 && !Step.bSkipIfNoTriggers)
		{
			UE_LOG(Game, Error, TEXT("A step has no trigger!"));
			bAllTriggersBound = false;
			break;
		}

		//for (AActor* Trigger : Step.Triggers)
		for (UActorComponent* Trigger : Step.Triggers)
		{
			// Break if a trigger is null (shouldn't be)
			if (Trigger == nullptr) 
			{
				UE_LOG(Game, Error, TEXT("A step has a trigger that is a nullptr!"));
				bAllTriggersBound = false;
				break;
			}
		}

		StepCount++;
	}

	// Clear our timer and setup the quest if all steps have triggers
	if (bAllTriggersBound)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitHandle);
		ReceiveSetupQuest();
		OnQuestBegin.Broadcast();
	}
}

// Finds and adds activators to a quest step
void AQuestManager::GetActivators()
{
	TArray<AActor*> InterfaceActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("UnboundActivator"), InterfaceActors);

	if (InterfaceActors.Num() > 0)
	{
		for (AActor* Actor : InterfaceActors)
		{
			UQuestActivatorComponent* Component = Actor->FindComponentByClass<UQuestActivatorComponent>();

			if (Component != nullptr && Component->BoundQuestKey == ActiveQuest.QuestKey)
			{
				int index = IQuestActivatorInterface::Execute_GetBoundStep(Component);

				if (index != -1 && ActiveQuest.QuestSteps.IsValidIndex(index))
				{
					ActiveQuest.QuestSteps[index].Activators.Add(Actor);
					Actor->Tags.Remove(FName("UnboundActivator"));
				}
			}
		}
	}
}

FQuestStep AQuestManager::BindStep(FQuestStep Step)
{
	Step.OnStepComplete.AddDynamic(this, &AQuestManager::NextStep);

	return Step;
}

void AQuestManager::BindQuest()
{
	ActiveQuest.OnQuestComplete.AddDynamic(this, &AQuestManager::ReceiveQuestFinished);

	ActiveQuest.ActiveStep = ActiveQuest.QuestSteps[0];
	ActiveQuest.OnActiveStepChanged.Broadcast(ActiveQuest.ActiveStep);
	EnableTriggers(ActiveQuest.ActiveStep);
	StepIndex = 0;
	ReceiveSetHudMessage();
}


void AQuestManager::NextStep()
{
	for (UActorComponent* Trigger : ActiveQuest.ActiveStep.Triggers)
	{
		// Set all triggers to active
		if (Trigger->GetClass()->ImplementsInterface(UQuestTriggerInterface::StaticClass()) && IQuestTriggerInterface::Execute_GetBoundStep(Trigger) == ActiveQuest.ActiveStep.StepNumber)
		{
			IQuestTriggerInterface::Execute_DisableTrigger(Trigger);
			IQuestTriggerInterface::Execute_UnbindTrigger(Trigger);
			UE_LOG(Game, Warning, TEXT("Cleared binding for trigger"))
		}
	
	}

	ActiveQuest.ActiveStep.OnStepComplete.Clear();
	DisableTriggers(ActiveQuest.ActiveStep);
	OnNextStep.Broadcast();

	// The StepIndex will indicate the step we just complete, unless this is the first step.
	// So, even after completing the step, the step doesn't advance until the "if" section of the if statmenet below
	// Set the new active step if we didn't just complete the last step of the quest
	// The first check is to ensure that we didn't forget to mark the last step with bFinalStep and accidentally check outside of the array bounds
	if (StepIndex != ActiveQuest.QuestSteps.Num() - 1 && !ActiveQuest.QuestSteps[StepIndex].bFinalStep)
	{
		StepIndex++;

		ActiveQuest.ActiveStep = ActiveQuest.QuestSteps[StepIndex];
		
		EnableTriggers(ActiveQuest.ActiveStep);

		ActiveQuest.OnActiveStepChanged.Broadcast(ActiveQuest.ActiveStep);
		
		ReceiveSetHudMessage();

		UE_LOG(Game, Warning, TEXT("%s"), *ActiveQuest.ActiveStep.DialogueText)
	}
	// If we just completed the final step, finish the quest
	else if (StepIndex == ActiveQuest.QuestSteps.Num() - 1 || ActiveQuest.QuestSteps[StepIndex].bFinalStep)
	{
		ActiveQuest.OnQuestComplete.Broadcast();
		OnQuestEnded.Broadcast();
		bQuestSetupComplete = false;
		UE_LOG(Game, Warning, TEXT("Completed Quest!"))
	}
	else
	{
		UE_LOG(Game, Error, TEXT("You shouldn't be here"))
	}
}

void AQuestManager::CompleteStep(USceneComponent* Trigger)
{	
	// Decrement the trigger count
	ActiveQuest.ActiveStep.BoundTriggerCount -= 1;
	ActiveQuest.ActiveStep.TriggersHit += 1;

	// Only move on if all of the triggers have been hit
	if (ActiveQuest.ActiveStep.BoundTriggerCount <= 0 || ActiveQuest.ActiveStep.TriggersHit >= ActiveQuest.ActiveStep.TriggersNeeded)
	{
		// Activate all activators linked to step
		if (ActiveQuest.ActiveStep.Activators.Num() > 0)
		{
			for (AActor* Activator : ActiveQuest.ActiveStep.Activators)
			{
				UQuestActivatorComponent* Component = Activator->FindComponentByClass<UQuestActivatorComponent>();

				if (Component != nullptr)
				{
					IQuestActivatorInterface::Execute_StartActivate(Component);
					UE_LOG(Game, Log, TEXT("Activated Activator"))
				}
				UE_LOG(Game, Error, TEXT("Actor does not implement UQuestActivatorInterface!"))
			}
		}

		UE_LOG(Game, Error, TEXT("No Activators linked to step!"))

		// This has to go after the activators are activated since it will advance the active step/end the quest
		ActiveQuest.ActiveStep.OnStepComplete.Broadcast();

		NextStep();
	}
	else
	{
		if (Trigger->GetClass()->ImplementsInterface(UQuestTriggerInterface::StaticClass()))
		{
			IQuestTriggerInterface::Execute_DisableTrigger(Trigger);
			IQuestTriggerInterface::Execute_UnbindTrigger(Trigger);
		}
	}
}

void AQuestManager::EnableTriggers(FQuestStep& QuestStep)
{
	for (UActorComponent* Trigger : QuestStep.Triggers)
	{
		// Set all triggers to active
		if (Trigger->GetClass()->ImplementsInterface(UQuestTriggerInterface::StaticClass()))
		{
			IQuestTriggerInterface::Execute_EnableTrigger(Trigger);
			UE_LOG(Game, Warning, TEXT("Activated trigger"))
		}
	}
}

void AQuestManager::DisableTriggers(FQuestStep& QuestStep)
{
	for (UActorComponent* Trigger : QuestStep.Triggers)
	{
		// Set all triggers to active
		if (Trigger->GetClass()->ImplementsInterface(UQuestTriggerInterface::StaticClass()))
		{
			IQuestTriggerInterface::Execute_DisableTrigger(Trigger);
			UE_LOG(Game, Warning, TEXT("Activated trigger"))
		}
	}
}

// If the step exists, get the step and return true
bool AQuestManager::GetStep(int StepNumber, struct FQuestStep& OutStruct)
{
	if (ActiveQuest.QuestSteps.IsValidIndex(StepNumber))
	{
		OutStruct = ActiveQuest.QuestSteps[StepNumber];
		return true;
	}
	else return false;
}

