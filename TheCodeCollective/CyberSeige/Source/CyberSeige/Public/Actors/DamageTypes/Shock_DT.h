// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Actors/Interfaces/EffectsInterface.h"
#include "Shock_DT.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API UShock_DT : public UDamageType, public IEffectsInterface
{
	GENERATED_BODY()

public:
	// Start IEffectsInterface
	virtual void StartEffect(AActor* DamagedActor, AActor* DamageCauser, float Damage) const override;
	// End IEffectsInterface

};
