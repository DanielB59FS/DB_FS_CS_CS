// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPlatform.h"
#include "Defenses.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/BoxComponent.h"

// Sets default values
ATurretPlatform::ATurretPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("P Mesh"));

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("P Bounding Box"));

	// Attach the components to the sockets
	SetRootComponent(Mesh);

	BoundingBox->SetupAttachment(Mesh);

	// Set variables
	HasPlacement = false;
	Mount = nullptr;
	Damage = 25;
}

// Called when the game starts or when spawned
void ATurretPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}
