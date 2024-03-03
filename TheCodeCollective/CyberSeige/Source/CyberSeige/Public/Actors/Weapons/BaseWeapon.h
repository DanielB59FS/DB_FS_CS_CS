// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/Enums.h"
#include "BaseWeapon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReload, UAnimSequenceBase*, Animation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShot, UAnimSequenceBase*, Animation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, float, currentAmmo, float, maxAmmo);


UCLASS()
class CYBERSEIGE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Base Weapon | Delegates")
		FOnReload OnReload;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Base Weapon | Delegates")
		FOnShot OnShot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Base Weapon | Delegates")
		FOnAmmoChanged OnAmmoChanged;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		TEnumAsByte<WeaponSlot> Slot;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		TEnumAsByte<WeaponType> Type;

	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		virtual void Fire(APawn* playerOwner) PURE_VIRTUAL(ABaseWeapon::Fire, ;);

	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		void SetCanFire();
	
	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		void RequestReload();
		
	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		void RequestFire(APawn* playerOwner);
		
	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		bool FireHitscan(APawn* playerOwner, FHitResult &outHitResult);
		
	UFUNCTION(BlueprintCallable, Category = "Base Weapon | Functions")
		bool CanFire();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		class UParticleSystemComponent* MuzzleParticles;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		class UParticleSystem* ImpactParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Weapon | Variables")
		float CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		float MaxAmmo;

	// Indicates how much ammo is used per bullet.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		float AmmoUsage;

	// Determines the delay between shots
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		float ShotDelay;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		float Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		float Range;

	// Indicates whether the weapon is automatic or not
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		bool IsAutomatic;

	// Animations for shooting and reloading
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		class UAnimSequenceBase* ReloadAnimation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		class UAnimSequenceBase* ShootAnimation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Base Weapon | Variables")
		bool WeaponCanFire;

private:
	bool isAnimating;
	FTimerHandle shootDelayTimer;
};
