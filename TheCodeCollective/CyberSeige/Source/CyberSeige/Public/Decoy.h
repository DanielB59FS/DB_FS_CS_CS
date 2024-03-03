// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "Decoy.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ADecoy : public ADefenses
{
	GENERATED_BODY()
	
public:
	ADecoy();

protected:

	void BeginPlay();
	void Activate();
	void Detonate();
	void SetOff();
	void SpawnEffect();

	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy")
	TSubclassOf<class ABaseAoeEffect> ExplosionClassType;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy")
	class USoundBase* PlacementSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy")
	class USoundBase* ActivateSound;

	float DetectRadius;
	float Delay;
	bool Active;


public:

	void Tick(float DeltaTime);

};
