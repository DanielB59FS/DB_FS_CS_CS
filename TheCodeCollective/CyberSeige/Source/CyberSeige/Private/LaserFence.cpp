// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserFence.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"

ALaserFence::ALaserFence() {

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the components
	Center = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LF Center"));
	LeftPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostMesh1"));
	RightPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PostMesh2"));
	LaserPSComponent1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LF LaserParticleSystemComponent1"));
	LaserPSComponent2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LF LaserParticleSystemComponent2"));
	LaserPSComponent3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LF LaserParticleSystemComponent3"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LF Bounding Box"));

	// Set root
	SetRootComponent(Center);

	// Attach the components to Root
	BoundingBox->SetupAttachment(Center);
	LeftPost->SetupAttachment(Center, TEXT("Left Post"));
	RightPost->SetupAttachment(Center, TEXT("Right Post"));

	// Attach Laser component
	LaserPSComponent1->SetupAttachment(LeftPost, TEXT("Laser1"));
	LaserPSComponent2->SetupAttachment(LeftPost, TEXT("Laser2"));
	LaserPSComponent3->SetupAttachment(LeftPost, TEXT("Laser3"));

	IsActive = true;
	Cooldown = 1.f;

}

void ALaserFence::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

	ActivateLasers();

}

void ALaserFence::ActivateLasers()
{

	// Initialize function variables
	FVector StartLocation;
	FVector EndLocation;

	// Calculate the start and end points
	StartLocation = LeftPost->GetSocketLocation(TEXT("Laser1"));
	EndLocation = RightPost->GetSocketLocation(TEXT("Laser1"));
	// Spawn the laser particle effect
	LaserPSComponent1 = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		LaserParticleSystemTemplate,
		StartLocation,
		LeftPost->GetForwardVector().Rotation()
	);
	// Start Visual Effect
	LaserPSComponent1->SetBeamSourcePoint(0, StartLocation, 0);
	LaserPSComponent1->SetBeamTargetPoint(0, EndLocation, 0);

	// Calculate the start and end points
	StartLocation = LeftPost->GetSocketLocation(TEXT("Laser2"));
	EndLocation = RightPost->GetSocketLocation(TEXT("Laser2"));
	// Spawn the laser particle effect
	LaserPSComponent2 = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		LaserParticleSystemTemplate,
		StartLocation,
		LeftPost->GetForwardVector().Rotation()
	);
	// Start Visual Effect
	LaserPSComponent2->SetBeamSourcePoint(0, StartLocation, 0);
	LaserPSComponent2->SetBeamTargetPoint(0, EndLocation, 0);

	// Calculate the start and end points
	StartLocation = LeftPost->GetSocketLocation(TEXT("Laser3"));
	EndLocation = RightPost->GetSocketLocation(TEXT("Laser3"));
	// Spawn the laser particle effect
	LaserPSComponent3 = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		LaserParticleSystemTemplate,
		StartLocation,
		LeftPost->GetForwardVector().Rotation()
	);
	// Start Visual Effect
	LaserPSComponent3->SetBeamSourcePoint(0, StartLocation, 0);
	LaserPSComponent3->SetBeamTargetPoint(0, EndLocation, 0);

	IsActive = true;
}

void ALaserFence::DeactivateLasers()
{
	IsActive = false;
	LaserPSComponent1->DeactivateImmediate();
	LaserPSComponent2->DeactivateImmediate();
	LaserPSComponent3->DeactivateImmediate();

}

void ALaserFence::DamageCheck()
{

	// Initialize function variables
	FVector StartLocation;
	FVector EndLocation;

	// Calculate the start and end points for the line trace
	StartLocation = LeftPost->GetSocketLocation(TEXT("Laser1"));
	EndLocation = RightPost->GetSocketLocation(TEXT("Laser1"));

	// Set up the trace parameters
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("FenceLaserTrace")), false, this);
	AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TraceParams.AddIgnoredActor(PlayerActor);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ObjectParams, TraceParams);

	if (bHit && IsValid(HitResult.GetActor())) {

		// Damage enemy
		HitResult.GetActor()->TakeDamage(100, FDamageEvent(), this->GetInstigatorController(), this);

		DeactivateLasers();

		// Restart Lasers timer
		GetWorld()->GetTimerManager().SetTimer(ResetTimerHandle, this, &ALaserFence::ActivateLasers, Cooldown, false);

	}

}

void ALaserFence::Tick(float DeltaTime)
{
	if (IsActive) {
		DamageCheck();
	}
}