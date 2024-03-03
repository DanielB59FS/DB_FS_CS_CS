// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"

#include "CustomCheatExtension.generated.h"

/**
 *
 */
UCLASS()
class CYBERSEIGE_API UCustomCheatExtension : public UCheatManagerExtension {
	GENERATED_BODY()

public:
	UCustomCheatExtension();

	//	Why can't I make use of Cheat Extension for methods?
	UFUNCTION(Exec, BlueprintCallable)
	static void MyTest(float val);
};
