// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API APistol : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void Fire(APawn* PlayerOwner) override;
};
