// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/BasePlayer.h"
#include "GameFramework/SpringArmcomponent.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "HUD/GameplayHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/Actor Components/WeaponHolster.h"
#include "Actors/Interfaces/ScrapSpawnerInterface.h"
#include "Animations/PlayerAnimEventGraph.h"
#include "Components/HealthComponent.h"
#include "Enums/Enums.h"
#include "Actors/Weapons/BaseWeapon.h"

ABasePlayer::ABasePlayer()
{
	// Create Components

}

void ABasePlayer::BeginPlay()
{
	WalkSpeed = 270.0f;
	RunSpeed = 540.0f;

	SetupHud();

	HealthComponent->OnDeath.AddDynamic(this, &ABasePlayer::PlayerDeath);
	HealthComponent->OnHealthChange.AddDynamic(GameplayHud, &UGameplayHUD::SetHealth);
}

void ABasePlayer::Tick(float deltaSeconds)
{
	UpdateAnimations();
}

UGameplayHUD* ABasePlayer::GetHUD()
{
	if (GameplayHud != nullptr)
		return GameplayHud;

	GameplayHud = CreateWidget<UGameplayHUD>(GetWorld(), GameplayHudClass);
	return GameplayHud;
}

bool ABasePlayer::CanPickupScrap_Implementation()
{
	return true;
}

void ABasePlayer::UpdateAnimations()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Cast to PlayerAnimEventGraph
	UPlayerAnimEventGraph* PlayerAnim = Cast<UPlayerAnimEventGraph>(AnimInstance);

	if (PlayerAnim != nullptr)
	{
		FVector Dir;
		float Speed;

		GetVelocity().ToDirectionAndLength(Dir, Speed);
		Speed *= ForwardAxis;
		
		PlayerAnim->Speed = Speed;
		PlayerAnim->Direction = (PlayerAnim->CalculateDirection(GetVelocity(), GetActorRotation()));
	}
}

void ABasePlayer::SetupHud()
{
	// Create our HUD if it doesn't exist and add it to the viewport
	GameplayHud = GetHUD();
	GameplayHud->AddToViewport();


}

void ABasePlayer::SetupAnimations()
{

}

void ABasePlayer::BindWeaponEvents(ABaseWeapon* Weapon)
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	UPlayerAnimEventGraph* PlayerAnim = Cast<UPlayerAnimEventGraph>(Anim);

	if (PlayerAnim == nullptr) return;

	Weapon->OnShot.AddDynamic(PlayerAnim, &UPlayerAnimEventGraph::PlayShootAnimation);
	Weapon->OnReload.AddDynamic(PlayerAnim, &UPlayerAnimEventGraph::PlayActionAnimation);
	PlayerAnim->OnReloadAnimationEnded.AddDynamic(Weapon, &ABaseWeapon::Reload);
	Weapon->OnAmmoChanged.AddDynamic(GameplayHud, &UGameplayHUD::SetAmmo);
	PlayerAnim->OnThrowableAnimationEnded.AddDynamic(this, &ABasePlayer::ThrowGrenade);
	WeaponHolster->OnThrowableUsed.AddDynamic(PlayerAnim, &UPlayerAnimEventGraph::PlayActionAnimation);
}

void ABasePlayer::PlayerDeath(float healthRatio)
{

}

// Movement and camera related
void ABasePlayer::MoveForward(float axisValue)
{
	ForwardAxis = axisValue;

	FRotator rotation = GetControlRotation();

	rotation = UKismetMathLibrary::MakeRotator(0, 0, rotation.Yaw);
	
	AddMovementInput(UKismetMathLibrary::GetForwardVector(rotation), axisValue);
}

void ABasePlayer::MoveRight(float axisValue)
{
	FRotator rotation = GetControlRotation();

	rotation = UKismetMathLibrary::MakeRotator(0, 0, rotation.Yaw);

	AddMovementInput(UKismetMathLibrary::GetRightVector(rotation), axisValue);
}

void ABasePlayer::ShiftPressed(FKey key)
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ABasePlayer::ShiftReleased(FKey key)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABasePlayer::LookRight(float axisValue)
{
	//if (HealthComponent.currentHealth > 0)
	//{
	AddControllerYawInput(axisValue);
	//}
}

void ABasePlayer::LookUp(float axisValue)
{
	//if (HealthComponent.currentHealth > 0)
	//{
	AddControllerPitchInput(axisValue);
	//}
}

// Weapon and grenade functions

void ABasePlayer::FireCurrentWeapon(bool pressed)
{

}

void ABasePlayer::FirePressed(FKey key)
{
	//if (BuildComponent.isActive)
	//{
	BuildComponentLeftClick();
	//}
	//else
		FireCurrentWeapon(true);
}

void ABasePlayer::FireReleased(FKey key)
{	//if (BuildComponent.isActive)
	//{
		BuildComponentLeftClick();
	//}
	//else
		FireCurrentWeapon(false);

}

void ABasePlayer::Reload(FKey key)
{

}

void ABasePlayer::AutomaticFire()
{

}

void ABasePlayer::Grenade(FKey key)
{
	//if (!BuildComponent.isActive)
	//{

	//}
}

void ABasePlayer::ThrowGrenade()
{

}


// Weapon holster related functions
void ABasePlayer::SwitchPrimary(FKey key)
{

}

void ABasePlayer::SwitchSecondary(FKey key)
{

}

void ABasePlayer::SwitchWeapon(float axisValue)
{

}


// Build Component Functions
void ABasePlayer::SwitchPlayerModes(FKey key)
{

}

void ABasePlayer::RotateDefensePlaceable(float axisValue)
{

}

void ABasePlayer::BuildComponentLeftClick()
{

}

void ABasePlayer::BuildComponentNext(float axisValue)
{

}

void ABasePlayer::BuildComponentRightClick(FKey key)
{

}
