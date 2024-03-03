// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "Blockades.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ABlockades : public ADefenses
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlockades();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blockade")
	UStaticMeshComponent* Wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blockade")
	class USoundBase* PlacementSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
