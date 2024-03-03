// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "TurretPlatform.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ATurretPlatform : public ADefenses
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	ATurretPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TSubclassOf<class ADefenses> Mount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	class USoundBase* PlacementSound;

	bool HasPlacement;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	UStaticMeshComponent* Mesh;

};
