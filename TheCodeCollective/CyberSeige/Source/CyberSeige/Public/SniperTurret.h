// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "SniperTurret.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ASniperTurret : public ADefenses
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASniperTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	APawn* Player;

	// Targeting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class UParticleSystem* LaserParticleSystemTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class UParticleSystemComponent* LaserPSComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		class USoundBase* LaserSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		FTimerHandle FireTimerHandle;

	FTimerHandle DestroyTimerHandle;
	FTimerHandle LTimerHandle;


	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	UStaticMeshComponent* Barrel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	class USoundBase* PlacementSound;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
	class UBoxComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
	class UBoxComponent* PlayerDetection;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
	class UTextRenderComponent* TextRenderer;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
	class UTextRenderComponent* TextRendererNot;


	FVector HitboxLocation;


	// Idling
	FRotator StartRot;
	FRotator LeftRot;
	FRotator RightRot;
	bool bRotation;
	bool Idling;
	float IdleTime;


	// Checks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		bool IsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
		bool IsFiring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
		bool Deactivated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		bool Repairing;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
		bool bTargetedByPlayer;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Turret")
		bool bPlayerCanAfford;

	float MaxRange;
	float FireCooldown;
	float LastShotTime;


	// Repairing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
		int RepairCost;


	// Functions
	UFUNCTION(BlueprintImplementableEvent)
	void FireAnimation();

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void InteractionText();
	UFUNCTION(BlueprintImplementableEvent)
	void Heal(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	bool CheckPlayerScrap(int cost);

	void ResetFire();
	void Idle(float DeltaTime);
	void FindTarget();
	void SetTargetHitbox();
	bool TargetInRange();
	void UpdateRotation(float DeltaTime);
	void TurretFire();
	void FireLaser();
	void DeactivateLaser();
	void Destruct();
	void Deactivate(float DeltaTime);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
