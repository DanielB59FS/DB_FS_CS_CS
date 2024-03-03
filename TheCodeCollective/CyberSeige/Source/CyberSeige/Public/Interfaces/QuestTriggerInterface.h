// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestTriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class CYBERSEIGE_API IQuestTriggerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		int GetBoundStep();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void EnableTrigger();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DisableTrigger();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UnbindTrigger();
};
