// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "Teleporter.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ATeleporter : public ADefenses
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleporter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	UStaticMeshComponent* Pad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	class USoundBase* PlacementSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	class USoundBase* TeleportSound;

	bool IsActive;
	bool Paired;
	float Cooldown;

	void Activate();

	void Deactivate();

	void PlayerCheck();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	class ATeleporter* PartnerTeleporter;

};
