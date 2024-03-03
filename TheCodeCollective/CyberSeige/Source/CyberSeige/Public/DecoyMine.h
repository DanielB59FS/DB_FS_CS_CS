// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "DecoyMine.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ADecoyMine : public ADefenses
{
	GENERATED_BODY()

public:

	ADecoyMine();

protected:

	void BeginPlay();
	void SpawnDecoy();

	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Decoy Mine")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy Mine")
	class USoundBase* PlacementSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy Mine")
	class USoundBase* ActivateSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decoy Mine")
	TSubclassOf<class AActor> DecoyClassType;

	UPROPERTY(BlueprintReadOnly, Category = "Decoy Mine")
	class ADecoy* Decoy;

	float DetectRadius;
	float Cooldown;
	float Delay;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Primed;

	
};
