// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponHolster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrowableUsed, UAnimSequenceBase*, Animation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnThrowableAmmoChanged, int, CurrentThrowableAmmo, int, MaxThrowableAmmo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERSEIGE_API UWeaponHolster : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponHolster();

	// Event Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon Holster | Delegates")
		FOnThrowableUsed OnThrowableUsed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Weapon Holster | Delegates")
		FOnThrowableAmmoChanged OnThrowableAmmoChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABaseWeapon* PrimaryWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABaseWeapon* SecondaryWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ABaseWeapon* CurrentWeapon;

private:

	UFUNCTION()
		ABaseWeapon* GetCurrentWeapon();

	UFUNCTION()
		bool CanUseThrowable();

	UFUNCTION()
		void ThrowableAnimationEnd();

	UPROPERTY(VisibleAnywhere)
		bool ThrowingGrenade;

	UPROPERTY(VisibleAnywhere)
		class ABaseWeapon* TempWeapon;

	UPROPERTY(VisibleAnywhere)
		int CurrentThrowableAmmo;

	UPROPERTY(VisibleAnywhere)
		int MaxThrowableAmmo;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void ReplaceWeapon(ABaseWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
		void GotoWeapon(int WeaponIndex);

	UFUNCTION(BlueprintCallable)
		void SwitchWeapons();

	UFUNCTION(BlueprintCallable)
		void ChangeWeapons(ABaseWeapon* WeaponOverride = nullptr);

	UFUNCTION(BlueprintCallable)
		void PlayThrowableAnimation();

	UPROPERTY(VisibleAnywhere)
		class UAnimSequenceBase* ThrowableAnimation;

};
