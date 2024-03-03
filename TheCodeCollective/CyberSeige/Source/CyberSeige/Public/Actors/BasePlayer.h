// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Interfaces/ScrapSpawnerInterface.h"
#include "BasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API ABasePlayer : public ABaseCharacter, public IScrapSpawnerInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

	ABasePlayer();

	UFUNCTION(BlueprintCallable, Category = "Base Player")
	UGameplayHUD* GetHUD();

	// Begin IScrapSpawnerInterface
	virtual bool CanPickupScrap_Implementation() override;
	// End IScrapSpawnerInterface

protected:
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Player")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Player")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Base Player")
		FName PrimarySocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Player")
		FName SecondarySocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Player | Components")
		class UWeaponHolster* WeaponHolster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base Player | Components")
		class UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Base Player")
		class UGameplayHUD* GameplayHud;

	UPROPERTY(EditDefaultsOnly, Category = "Base Player")
		TSubclassOf<UGameplayHUD> GameplayHudClass;

private:
	UPROPERTY(VisibleAnywhere, Category = "Base Player | Variables")
		float ForwardAxis;

	UPROPERTY(VisibleAnywhere, Category = "Base Player | Variables")	
		float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Base Player | Variables")
		float RunSpeed;

	// HUD, player, and weapon setup
	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void UpdateAnimations();

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SetupHud();

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SetupAnimations();

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void BindWeaponEvents(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void PlayerDeath(float healthRatio);

	// Movement and camera related
	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void MoveForward(float axisValue);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void MoveRight(float axisValue);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void ShiftPressed(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void ShiftReleased(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void LookRight(float axisValue);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void LookUp(float axisValue);

	// Weapon and grenade functions
	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void FireCurrentWeapon(bool pressed);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void FirePressed(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void FireReleased(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void Reload(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void AutomaticFire();

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void Grenade(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void ThrowGrenade();

	// Weapon holster related functions
	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SwitchPrimary(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SwitchSecondary(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SwitchWeapon(float axisValue);

	// Build Component Functions
	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void SwitchPlayerModes(FKey key);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void RotateDefensePlaceable(float axisValue);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void BuildComponentLeftClick();

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void BuildComponentNext(float axisValue);

	UFUNCTION(BlueprintCallable, Category = "Base Player | Functions")
		void BuildComponentRightClick(FKey key);
};
