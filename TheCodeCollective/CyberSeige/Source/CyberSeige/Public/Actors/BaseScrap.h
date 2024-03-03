// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseScrap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScrapCollected);

UCLASS()
class CYBERSEIGE_API ABaseScrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseScrap();

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		TSubclassOf<AActor> ScrapPieceClass;

	UFUNCTION(BlueprintCallable, Category = "Base Scrap")
		void SetScrapAmount(float NewAmount);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnScrapCollected OnScrapCollected;

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
		float CogHoverRads;

	UPROPERTY()
		float CogRotRads;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		float CogRotSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		float CogPosSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		class UStaticMesh* ScrapPieceMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		class UStaticMeshComponent* CogMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		class UNiagaraComponent* NiagaraParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		class USphereComponent* DetectionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Base Scrap")
		class AActor* ScrapTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Base Scrap")
		int ScrapAmount;

	UFUNCTION(BlueprintCallable, Category = "Base Scrap")
		void Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Base Scrap")
		void SpawnScrap();
	
	FTimerHandle handle;
};
