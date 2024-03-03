// Fill out your copyright notice in the Description page of Project Settings.


#include "Decoy.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADecoy::ADecoy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("D Mesh"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("D Bounding Box"));

	// Attach the components to the sockets
	SetRootComponent(Mesh);

	BoundingBox->SetupAttachment(Mesh);

	Delay = 10;
	DetectRadius = 300;
	Active = false;

}

void ADecoy::BeginPlay()
{
	Super::BeginPlay();

}

void ADecoy::Activate()
{
	// Start moving up then Detonate
	SetActorLocation(GetActorLocation()+FVector(0.f, 0.f, -25.f));

	// Self Destruct
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ADecoy::Detonate, Delay, false);

}

void ADecoy::Detonate()
{

	ABaseAoeEffect* Explosion = GetWorld()->SpawnActor<ABaseAoeEffect>(ExplosionClassType, Mesh->GetComponentLocation(), Mesh->GetForwardVector().Rotation());
	Destroy();

}

void ADecoy::SetOff()
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

				Active = true;
				Activate();

				return;

			}
		}
	}

}

void ADecoy::SpawnEffect()
{

	ABaseAoeEffect* Explosion = GetWorld()->SpawnActor<ABaseAoeEffect>(ExplosionClassType, Mesh->GetComponentLocation(), Mesh->GetForwardVector().Rotation());

}


void ADecoy::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	SetActorTickInterval(0.5);

	if (!Active) {

		SetOff();

	}

}

