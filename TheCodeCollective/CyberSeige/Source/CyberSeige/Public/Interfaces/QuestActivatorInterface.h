// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestActivatorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQuestActivatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYBERSEIGE_API IQuestActivatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StartActivate();

	// This function is used by the QuestManager for activators that are linked to steps afte runtime
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		int GetBoundStep();

};
