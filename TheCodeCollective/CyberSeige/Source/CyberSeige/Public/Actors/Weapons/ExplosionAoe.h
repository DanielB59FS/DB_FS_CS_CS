// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "ExplosionAoe.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API AExplosionAoe : public ABaseAoeEffect
{
	GENERATED_BODY() 
	
public: 
	AExplosionAoe();

protected:
	virtual void ApplyEffect(AActor* target) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Eplxosion AOE | Varaibles")
		float Damage;

};
