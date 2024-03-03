// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class CYBERSEIGE_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Components")
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Components")
		class USphereComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Components")
		class UParticleSystemComponent* Particles;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Components")
		class UStaticMeshComponent* Mesh;

	// Used if projectile spawns an AOE
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Variables")
		TSubclassOf<class ABaseAoeEffect> AoeEffect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Variables")
		bool SpawnsAoe;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Variables")
		bool ActivatesOnImpact;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Projectile | Variables")
		float DelayAmount;

	FTimerHandle DelayTimerHandle;

private:
	UFUNCTION(Category = "Base Projectile | Functions")
		void HandleImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector Impulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Base Projectile | Functions")
		void SetupTimer();

	UFUNCTION(BlueprintCallable, Category = "Base Projectile | Functions")
		void SpawnAoe();
};
