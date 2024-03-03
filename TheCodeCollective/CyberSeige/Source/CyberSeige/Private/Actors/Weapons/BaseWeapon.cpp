// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Animation/AnimSequenceBase.h"
#include "Enums/Enums.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MuzzleParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));

	// Setup Attachments
	Mesh->SetupAttachment(RootComponent);
	MuzzleParticles->SetupAttachment(RootComponent);

	// Define variables
	CurrentAmmo = 0.0f;
	MaxAmmo = 10.0f;
	AmmoUsage = 1.0f;
	ShotDelay = 0.1f;
	Type = WeaponType::Unarmed;
	Slot = WeaponSlot::Primary;
	ImpactParticles = nullptr;
	WeaponCanFire = true;
	IsAutomatic = false;
	Damage = 1.0f;
	ReloadAnimation = nullptr;
	ShootAnimation = nullptr;
	isAnimating = false;
	Range = 1000.0f;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// Set current ammo to max
	CurrentAmmo = MaxAmmo;

}

void ABaseWeapon::SetCanFire()
{
	WeaponCanFire = true;
}

void ABaseWeapon::RequestReload()
{
	if (CanFire())
	{
		if (ReloadAnimation != nullptr)
		{
			OnReload.Broadcast(ReloadAnimation);
			isAnimating = true;
		}
		else
			Reload();
	} 

}

void ABaseWeapon::Reload()
{
	CurrentAmmo = MaxAmmo;
	OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
	isAnimating = false;
}

void ABaseWeapon::RequestFire(APawn* playerOwner)
{
	if (CanFire() && CurrentAmmo > 0)
	{
		CurrentAmmo -= AmmoUsage;
		MuzzleParticles->Activate(true);

		Fire(playerOwner);

		GetWorldTimerManager().SetTimer(shootDelayTimer, this, &ABaseWeapon::SetCanFire, ShotDelay, false, 0.0f);
	
		OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);

		if (ShootAnimation != nullptr)
		{
			OnShot.Broadcast(ShootAnimation);
		}
	}
}

bool ABaseWeapon::FireHitscan(APawn* playerOwner, FHitResult& outHitResult)
{
	FVector cameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector forwardRange = UKismetMathLibrary::GetForwardVector(playerOwner->GetBaseAimRotation()) * Range;
	FVector endPos = cameraLocation + forwardRange;

	const TArray<TEnumAsByte<EObjectTypeQuery>> Objects = { UEngineTypes::ConvertToObjectType(ECC_WorldStatic), UEngineTypes::ConvertToObjectType(ECC_Pawn) };
	const TArray<AActor*> ignoreList = { playerOwner };

	bool didHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), cameraLocation, endPos, Objects, false, ignoreList, EDrawDebugTrace::ForDuration, outHitResult, true);

	if (didHit)
	{
		// Do Damage here
		if (outHitResult.Actor != nullptr)
		{
			UGameplayStatics::ApplyDamage(outHitResult.GetActor(), Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, outHitResult.ImpactPoint);
	}

	return didHit;
}

bool ABaseWeapon::CanFire()
{
	return !isAnimating && WeaponCanFire;
}


