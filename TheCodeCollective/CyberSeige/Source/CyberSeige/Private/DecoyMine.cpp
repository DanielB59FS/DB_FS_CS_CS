// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoyMine.h"
#include "Decoy.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
ADecoyMine::ADecoyMine()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DM Mesh"));
	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DM Bounding Box"));

	SetRootComponent(Mesh);

	// Setup attachments
	BoundingBox->SetupAttachment(Mesh);

	// Define variables
	DetectRadius = 100;
	Primed = false;
	Cooldown = 15;

}

// Called when the game starts or when spawned
void ADecoyMine::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

	// Timer to spawn decoy
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ADecoyMine::SpawnDecoy, Cooldown, false);

}

void ADecoyMine::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (Primed && !Decoy) {

		Primed = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ADecoyMine::SpawnDecoy, Cooldown, false);

	}

}

void ADecoyMine::SpawnDecoy()
{

	Decoy = GetWorld()->SpawnActor<ADecoy>(DecoyClassType, Mesh->GetSocketLocation(TEXT("Decoy")), Mesh->GetForwardVector().Rotation());
	Primed = true;

}

