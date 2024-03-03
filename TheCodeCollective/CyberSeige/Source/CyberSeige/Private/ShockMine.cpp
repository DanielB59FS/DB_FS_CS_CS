// Fill out your copyright notice in the Description page of Project Settings.


#include "ShockMine.h"
#include "Mines.h"
#include "Actors/DamageTypes/Shock_DT.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AShockMine::AShockMine()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM Mesh"));
	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SM Bounding Box"));

	SetRootComponent(Mesh);

	// Setup attachments
	BoundingBox->SetupAttachment(Mesh);

	// Define variables
	DmgRadius = 400;
	DetectRadius = 100;
	Primed = true;
	Delay = 0.5f;
	Cooldown = 10;
	Damage = 1;

}

// Called when the game starts or when spawned
void AShockMine::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}

// helper function to bind to the timer
void AShockMine::SpawnEffect()
{

	// Spawn explosion -- Change it to a shock one
	ABaseAoeEffect* Explosion = GetWorld()->SpawnActor<ABaseAoeEffect>(ExplosionClassType, Mesh->GetComponentLocation(), Mesh->GetForwardVector().Rotation());

}

void AShockMine::SetOff()
{

	// Define the sphere trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, this);
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TraceParams.AddIgnoredActor(PlayerActor);
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel1;
	TArray<FHitResult> HitResults;

	// Perform the sphere trace targeting pawns
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * DetectRadius), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(DetectRadius), TraceParams);

	for (int HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx) {
		AActor* HitActor = HitResults[HitIdx].GetActor();

		if (HitActor) {
			APawn* TargetPawn = Cast<APawn>(HitActor);

			if (TargetPawn) {

				Primed = false;

				GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AShockMine::Detonate, Delay, false);

				GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AShockMine::RePrime, Cooldown, false);

				return;

			}
		}
	}

}

void AShockMine::Detonate()
{

	// Define the sphere trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, this);
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel1;
	TArray<FHitResult> HitResults;

	// Perform the sphere trace targeting pawns
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * DmgRadius), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(DmgRadius), TraceParams);

	SpawnEffect();

	for (int HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx) {
		AActor* HitActor = HitResults[HitIdx].GetActor();

		if (HitActor) {
			APawn* TargetPawn = Cast<APawn>(HitActor);

			if (TargetPawn) {

				// Damage enemy and Apply effect
				UGameplayStatics::ApplyDamage(TargetPawn, Damage, GetInstigatorController(), this, UShock_DT::StaticClass());

			}

		}
	}

}

void AShockMine::RePrime()
{

	Primed = true;

}

void AShockMine::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (Primed) {

		SetOff();

	}

}


