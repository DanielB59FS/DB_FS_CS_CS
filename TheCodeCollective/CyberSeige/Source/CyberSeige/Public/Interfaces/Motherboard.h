// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Motherboard.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMotherboard : public UInterface {
	GENERATED_BODY()
};

/**
 *
 */
class CYBERSEIGE_API IMotherboard {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TArray<class IChip*>* GetChips() = 0;
	virtual int GetChipCount() = 0;
	virtual bool EquipChip(class IChip* chip) = 0;
	virtual void RemoveChip(int) = 0;
	virtual float GetSlotCap() = 0;
	virtual void SetSlotCap(float cap) = 0;
	virtual float GetSlotUsage() = 0;
	virtual void ApplyPenalty(class ABaseCharacter*) = 0;
	virtual bool IsOverclocked() = 0;
};
