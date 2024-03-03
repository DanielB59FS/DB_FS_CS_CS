// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "ShockMine.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API AShockMine : public ADefenses
{
	GENERATED_BODY()

public:

	AShockMine();

protected:

	void BeginPlay();
	void SpawnEffect();
	void SetOff();
	void RePrime();

	FTimerHandle CooldownTimerHandle;
	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shock Mine")
	TSubclassOf<class AActor> EffectClassType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shock Mine")
	TSubclassOf<class ABaseAoeEffect> ExplosionClassType;

	UPROPERTY(EditDefaultsOnly, Category = "Shock Mine")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shock Mine")
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
