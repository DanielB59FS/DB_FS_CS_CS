// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestTriggerComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameModes/QuestManager.h"
#include "../../CyberSeige.h"

// Sets default values for this component's properties
UQuestTriggerComponent::UQuestTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bTriggerActive = false;

	BoundStep = -1;
}


// Called when the game starts
void UQuestTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (BoundStep != -1)
	{
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &UQuestTriggerComponent::WaitForQuestManager, 1, true, 0);
	}
}

void UQuestTriggerComponent::WaitForQuestManager()
{
	//// Triggers are automatically bound if they are available before runtime
	//// If they are not available before runtime, we have to bind them dynamically
	//// If the BoundStep isn't the default value, look to bind it to a step
	//if (BoundStep != -1)
	//{
	//	AActor* QuestManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AQuestManager::StaticClass());
	//	AQuestManager* QuestManager = Cast<AQuestManager>(QuestManagerActor);

	//	if (QuestManager != nullptr)
	//	{
	//		FQuestStep Step;
	//		AActor* TriggerActor = this->GetOwner();
	//		if (QuestManager->GetStep(BoundStep, Step) && TriggerActor != nullptr && QuestManager->ActiveQuest.QuestKey == BoundQuestKey)
	//		{
	//			if (QuestManager->ActiveQuest.QuestSteps[BoundStep].AddTrigger(TriggerActor))
	//			{
	//				GetWorld()->GetTimerManager().ClearTimer(WaitTimer);
	//				UE_LOG(Game, Display, TEXT("Bound Trigger!"));
	//			}
	//			else UE_LOG(Game, Error, TEXT("Failed to bind trigger to step! Trigger may be nullptr"));
	//		}
	//		else
	//			UE_LOG(Game, Error, TEXT("Step number %i does not exist, or the QuestTrigger is not attached to an actor"), BoundStep);
	//	}
	//	else
	//	{
	//		UE_LOG(Game, Error, TEXT("Could not find QuestManager! Cannot proceed"));
	//	}
	//}
}

void UQuestTriggerComponent::CompleteStep()
{
	// Don't complete the step if the trigger isn't active
	if (!bTriggerActive) return;

	OnCompleteStep.Broadcast();
	bTriggerActive = false;
}

