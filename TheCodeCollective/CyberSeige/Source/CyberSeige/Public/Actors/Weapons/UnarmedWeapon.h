// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "UnarmedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API AUnarmedWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
};
