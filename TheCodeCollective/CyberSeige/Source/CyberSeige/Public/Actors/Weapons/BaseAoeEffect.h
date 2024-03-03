// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAoeEffect.generated.h"

UCLASS()
class CYBERSEIGE_API ABaseAoeEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAoeEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Base AOE | Functions")
		virtual void ApplyEffect(AActor* target) PURE_VIRTUAL(ABaseAoeEffect::ApplyEffect, ;);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base AOE | Components")
		class USphereComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base AOE | Components")
		class UParticleSystemComponent* Particles;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base AOE | Variables")
		class USoundCue* SoundEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(Category = "Base AOE | Functions")
		void DestroyAoe(UParticleSystemComponent* ParticlesToDestroy);

};