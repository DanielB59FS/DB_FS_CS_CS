// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDispenser.generated.h"

UCLASS()
class CYBERSEIGE_API AWeaponDispenser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDispenser();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Components")
		class UStaticMeshComponent* DispenserMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Components")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Components")
		class UBoxComponent* Collision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Components")
		class UBoxComponent* PlayerDetection;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Components")
		class UTextRenderComponent* TextRenderer;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon Dispenser | Functions")
		AActor* DispenseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon Dispenser | Functions")
		void DisplayText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Weapon Dispenser | Variables")
		class AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon Dispenser | Variables")
		float WeaponHoverRads;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon Dispenser | Variables")
		float WeaponRotateDegrees;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Variables")
		float RotationSpeed;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Variables")
		float HoverSpeed;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Dispenser | Variables")
		bool bTargetedByPlayer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
