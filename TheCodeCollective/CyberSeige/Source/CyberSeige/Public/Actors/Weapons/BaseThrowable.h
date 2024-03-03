// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseThrowable.generated.h"

UCLASS()
class CYBERSEIGE_API ABaseThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseThrowable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SpawnEffect(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SpawnEffectNoArgs();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Throwable")
		float Delay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Throwable")
		class ABaseAoeEffect* Effect;

	UPROPERTY(EditDefaultsOnly, Category = "Base Throwable")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Throwable")
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "Base Throwable")
		class UParticleSystemComponent* Trail;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Throwable")
	bool ExplodeOnImpact;

};
