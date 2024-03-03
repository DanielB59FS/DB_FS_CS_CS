// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "Mines.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API AMines : public ADefenses
{
	GENERATED_BODY()
	
public:

	AMines();

protected:

	void BeginPlay();
	void SpawnEffect();
	void SetOff();
	void RePrime();
	void DetonateMines();

	FTimerHandle CooldownTimerHandle;
	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	TSubclassOf<class ABaseAoeEffect> ExplosionClassType;

	UPROPERTY(EditDefaultsOnly, Category = "Mine")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
	class USoundBase* PlacementSound;

	float DmgRadius;
	float DetectRadius;
	float Cooldown;
	float Delay;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Primed;
	void Detonate();

};

