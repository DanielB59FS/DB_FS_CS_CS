// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShockEffectActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShockActorEnded);

UCLASS()
class CYBERSEIGE_API AShockEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShockEffectActor();

	// Event delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FOnShockActorEnded OnShockActorEnded;

	UFUNCTION(BlueprintCallable)
		void SetShockParameters(int SpreadCharges = 10, int ShockCharges = 3, float ShockInterval = 2.5f, float ShockDamage = 5.0f);

	UFUNCTION(BlueprintCallable)
		void SetShockedList(TSet<AActor*> NewList);

	UFUNCTION(BlueprintCallable)
		TSet<AActor*> GetShockedList();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect | Parameters")
		int SpreadCharges;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect | Parameters")
		int ShockCharges;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect | Parameters")
		float ShockInterval;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect | Parameters")
		float ShockDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Effect | Components")
		class UNiagaraComponent* NiagaraParticles;

	UPROPERTY(EditAnywhere, Category = "Effect | Components")
		class USphereComponent* Collision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void ApplyShock();

	// Forward declare for damage type
	UPROPERTY()
		TSubclassOf<class UShock_DT> ShockDT;

	UPROPERTY()
		TSet<AActor*> ShockedList;

	UPROPERTY()
		FTimerHandle ShockTimerHandle;
};
