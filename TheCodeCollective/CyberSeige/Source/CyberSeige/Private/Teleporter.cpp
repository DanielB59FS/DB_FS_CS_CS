// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleporter.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"

ATeleporter::ATeleporter()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Pad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TP Mesh"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TP Bounding Box"));

	// Attach the components to the sockets
	SetRootComponent(Pad);

	BoundingBox->SetupAttachment(Pad);

	IsActive = false;
	Paired = false;
	Cooldown = 20;

}

void ATeleporter::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}

void ATeleporter::Activate()
{
	
	IsActive = true;

	// Start effect

}

void ATeleporter::Deactivate()
{

	IsActive = false;

	// End effect


	// Cooldown Timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATeleporter::Activate, Cooldown, false);
}

void ATeleporter::PlayerCheck()
{

	// Ray cast sphere above for player// Define the sphere trace parameters
	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(FName(TEXT("SphereTrace")), false, this);
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;

	// Perform the sphere trace targeting pawns
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation() + FVector(0.f, 0.f, 100.f), FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(100), TraceParams);

	for (int HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx) {
		AActor* HitActor = HitResults[HitIdx].GetActor();

		if (HitActor) {
			APawn* TargetPawn = Cast<APawn>(HitActor);

			if (TargetPawn == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) {

				// Play teleport sound
				UGameplayStatics::SpawnSoundAtLocation(this, TeleportSound, PartnerTeleporter->GetActorLocation());
				UGameplayStatics::SpawnSoundAtLocation(this, TeleportSound, GetActorLocation());

				Deactivate();
				PartnerTeleporter->Deactivate();

				// Change Player location by adding the difference of the teleporters to the player and keeping rotation. 
				FHitResult HitResult;
				TargetPawn->SetActorLocation(TargetPawn->GetActorLocation() + (PartnerTeleporter->GetActorLocation() - GetActorLocation()), false, &HitResult, ETeleportType::TeleportPhysics);

				return;

			}
		}
	}

}

void ATeleporter::Tick(float DeltaTime)
{

	if (!Paired && PartnerTeleporter) {

		// Activation Delay
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATeleporter::Activate, 2.f, false);
		Paired = true;

	}

	if (IsActive) {

		PlayerCheck();

	}

}
