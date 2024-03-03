// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Defenses.h"
#include "LaserFence.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ALaserFence : public ADefenses
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaserFence();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle ResetTimerHandle;

	// Laser timers
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;
	FTimerHandle TimerHandle3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		UStaticMeshComponent* LeftPost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		UStaticMeshComponent* RightPost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		UStaticMeshComponent* Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		class UParticleSystem* LaserParticleSystemTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserFence")
		class UParticleSystemComponent* LaserPSComponent1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserFence")
		class UParticleSystemComponent* LaserPSComponent2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LaserFence")
		class UParticleSystemComponent* LaserPSComponent3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		class USoundBase* PlacementSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserFence")
		class USoundBase* LaserSound;

	bool IsActive;
	float Cooldown;
	void ActivateLasers();
	void DeactivateLasers();
	void DamageCheck();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
