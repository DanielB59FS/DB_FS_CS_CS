// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Enums.generated.h"

/**
 * 
 */

// Used for dictating the player animations
UENUM(BlueprintType)
enum PlayerAnimationState
{
	Alive		UMETA(DisplayName = "Alive"),
	Dead		UMETA(DisplayName = "Dead")
};

// Enum for keeping track of the weapon. Used for attaching weapons to the player
UENUM(BlueprintType)
enum WeaponSlot
{
	Primary		UMETA(DisplayName = "Primary"),
	Secondary	UMETA(DisplayName = "Secondary"),
	Throwable	UMETA(DisplayName = "Throwable")
};

// Enum for different effects in the game
UENUM(BlueprintType)
enum EEffects
{
	Shocking		UMETA(DisplayName = "Shocking")
};

// Enum for keeping track of the weapon. Used for animation switching
UENUM(BlueprintType)
enum WeaponType
{
	Unarmed			UMETA(DisplayName = "Unarmed"),
	Pistol			UMETA(DisplayName = "Pistol"),
	AssaultRifle	UMETA(DisplayName = "AssaultRifle"),
	LightMachineGun	UMETA(DisplayName = "LightMachineGun"),
	Sniper			UMETA(DisplayName = "Sniper"),
	SubMachineGun	UMETA(DisplayName = "SubMachineGun"),
	RocketLauncher	UMETA(DisplayName = "RocketLauncher"),
	FreezeGun		UMETA(DisplayName = "FreezeGun"),
	ShockGun		UMETA(DisplayName = "ShockGun"),
	RepairGun		UMETA(DisplayName = "RepairGun")
};

// Enum for the different types of defenses. Used for enambling/disabling items
UENUM(BlueprintType)
enum EDefenseType
{
	Blockade		UMETA(DisplayName = "Blockade"),
	Turret			UMETA(DisplayName = "Turret"),
	Trap			UMETA(DisplayName = "Trap")
};

UCLASS()
class CYBERSEIGE_API UEnums : public UUserDefinedEnum
{
	GENERATED_BODY()

};
