// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Actor Components/WeaponHolster.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Animation/AnimSequenceBase.h"
#include "Actors/Weapons/UnarmedWeapon.h"

// Sets default values for this component's properties
UWeaponHolster::UWeaponHolster()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

}

// Called when the game starts
void UWeaponHolster::BeginPlay()
{
	Super::BeginPlay();

	MaxThrowableAmmo = 3;
	CurrentThrowableAmmo = MaxThrowableAmmo;

	OnThrowableAmmoChanged.Broadcast(CurrentThrowableAmmo, MaxThrowableAmmo);
}

// Called every frame
void UWeaponHolster::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UWeaponHolster::CanUseThrowable()
{
	return (CurrentThrowableAmmo > 0 && !ThrowingGrenade);
}

void UWeaponHolster::PlayThrowableAnimation()
{
	ThrowingGrenade = true;

	// Set our current weapon to temp wepaon
	TempWeapon = CurrentWeapon;

	// Override our current weapon with an unarmed weapon that can't fire
	ChangeWeapons(GetWorld()->SpawnActor<AUnarmedWeapon>());

	OnThrowableUsed.Broadcast(ThrowableAnimation);

	CurrentThrowableAmmo--;

	OnThrowableAmmoChanged.Broadcast(CurrentThrowableAmmo, MaxThrowableAmmo);
}

void UWeaponHolster::ThrowableAnimationEnd()
{
	if (Cast<AUnarmedWeapon>(CurrentWeapon))
	{
		CurrentWeapon->Destroy();
	}

	ChangeWeapons(TempWeapon);

	ThrowingGrenade = false;
}

// Replaces a weapon in the holster with a new weapon.
// Call this if the player picks up a new wepaon in some fashion.
void UWeaponHolster::ReplaceWeapon(ABaseWeapon* NewWeapon)
{
	if (ThrowingGrenade) return;

	if (NewWeapon != nullptr)
	{
		TEnumAsByte<WeaponSlot> slot = NewWeapon->Slot;

		switch (slot)
		{
		case WeaponSlot::Primary:
			PrimaryWeapon = NewWeapon;
			CurrentWeapon = PrimaryWeapon;
			ChangeWeapons(PrimaryWeapon);
			break;
		case WeaponSlot::Secondary:
			SecondaryWeapon = NewWeapon;
			CurrentWeapon = SecondaryWeapon;
			ChangeWeapons(SecondaryWeapon);
			break;
		default:
			break;
		}
	}
}

// Swaps the current weapon for whatever weapon is at the specified index.
// Valid indexes are 0 and 1 for Primary and Secondary.
void UWeaponHolster::GotoWeapon(int WeaponIndex)
{
	if (ThrowingGrenade) return;

	switch (WeaponIndex)
	{
	case 0:
		ChangeWeapons(PrimaryWeapon);
		break;
	case 1:
		ChangeWeapons(SecondaryWeapon);
		break;
	default:
		break;
	}
}

// Generally switches wepaons based on what is currently equipped.
void UWeaponHolster::SwitchWeapons()
{
	if (ThrowingGrenade) return;

	ChangeWeapons();
}

// Helper function for SwitchWeapons and GotoWeapon.
// WeaponOverride is optional and is used primarily for GotoWeapon
void UWeaponHolster::ChangeWeapons(ABaseWeapon* WeaponOverride)
{
	// Swap to weapon override if it is valid.
	if (WeaponOverride != nullptr)
	{
		CurrentWeapon = WeaponOverride;
		return;
	}

	// We have to check a few things when switching wepaons.
	// - Is our current weapon valid
	//		Yes: Switch to whatever isn't current if its valid
	//		No: This means we have nothing equipped. Find the first weapon that is valid and equip it
	if (CurrentWeapon != nullptr && CurrentWeapon->Slot == WeaponSlot::Primary)
	{
		if (SecondaryWeapon != nullptr)
		{
			CurrentWeapon = SecondaryWeapon;
		}
	}
	else if (CurrentWeapon != nullptr && CurrentWeapon->Slot == WeaponSlot::Secondary)
	{
		if (PrimaryWeapon != nullptr)
		{
			CurrentWeapon = PrimaryWeapon;
		}
	}
	else if (CurrentWeapon == nullptr)
	{
		if (PrimaryWeapon != nullptr) CurrentWeapon = PrimaryWeapon;
		else if (SecondaryWeapon != nullptr) CurrentWeapon = SecondaryWeapon;
	}
}

// Returns the current weapon equipped.
// Used for the player to request a reference to their current wepaon.
ABaseWeapon* UWeaponHolster::GetCurrentWeapon()
{
	return CurrentWeapon;
}




