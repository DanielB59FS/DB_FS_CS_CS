// Fill out your copyright notice in the Description page of Project Settings.


#include "Mines.h"
#include "ShockMine.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AMines::AMines()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("M Mesh"));
	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("M Bounding Box"));

	SetRootComponent(Mesh);

	// Setup attachments
	BoundingBox->SetupAttachment(Mesh);

	// Define variables
	DmgRadius = 350;
	DetectRadius = 100;
	Primed = true;
	Delay = 0.5f;
	Cooldown = 10;
	Damage = 150;

}

// Called when the game starts or when spawned
void AMines::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}

// helper function to bind to the timer
void AMines::SpawnEffect()
{

	ABaseAoeEffect* Explosion = GetWorld()->SpawnActor<ABaseAoeEffect>(ExplosionClassType, Mesh->GetComponentLocation(), Mesh->GetForwardVector().Rotation());

}

void AMines::SetOff()
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

				GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AMines::Detonate, Delay, false);

				GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AMines::RePrime, Cooldown, false);

				return;

			}
		}
	}

}

void AMines::Detonate()
{
	Primed = false;

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

				// Damage enemy
				TargetPawn->TakeDamage(Damage, FDamageEvent(), this->GetInstigatorController(), this);

			}

		}
	}

	DetonateMines();

}

void AMines::DetonateMines()
{

	// Define the sphere trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, this);
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel3;
	TArray<FHitResult> HitResults;

	// Perform the sphere trace targeting pawns
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * DmgRadius), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(DmgRadius), TraceParams);

	for (int HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx) {
		AActor* HitActor = HitResults[HitIdx].GetActor();

		if (HitActor) {

			AMines* TargetMine = Cast<AMines>(HitActor);
			AShockMine* TargetShockMine = Cast<AShockMine>(HitActor);

			if (TargetMine && TargetMine->Primed) {

				TargetMine->Detonate();

			}
			else if (TargetShockMine && TargetShockMine->Primed) {

				TargetShockMine->Detonate();

			}

		}
	}

}

void AMines::RePrime()
{

	Primed = true;

}

void AMines::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (Primed) {

		SetOff();

	}

}

