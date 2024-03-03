// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScrapPiece.generated.h"

UCLASS()
class CYBERSEIGE_API AScrapPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScrapPiece();

	UFUNCTION(BlueprintCallable)
		void SetHomingTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable)
		void SetMesh(UStaticMesh* NewMesh);

	UFUNCTION(BlueprintCallable)
		int GetScrapValue();

	UFUNCTION(BlueprintCallable)
		void SetScrapValue(int NewScrapValue);

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* PieceMesh;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere)
		int ScrapValue;

	UFUNCTION(BlueprintCallable)
		void SetHomingTrue();

};
