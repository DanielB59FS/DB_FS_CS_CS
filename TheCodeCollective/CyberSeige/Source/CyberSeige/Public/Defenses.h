// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Defenses.generated.h"

UCLASS()
class CYBERSEIGE_API ADefenses : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenses();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		class UStaticMesh* WholeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		class UBoxComponent* BoundingBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
		int MaxHealth;

	bool OnPlatform;
};
