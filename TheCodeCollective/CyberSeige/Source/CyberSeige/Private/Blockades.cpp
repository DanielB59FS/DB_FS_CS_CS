// Fill out your copyright notice in the Description page of Project Settings.


#include "Blockades.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
ABlockades::ABlockades()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the components
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("B Bounding Box"));

	// Set root
	SetRootComponent(Wall);

	// Attach the components to Root
	BoundingBox->SetupAttachment(Wall);

}

// Called when the game starts or when spawned
void ABlockades::BeginPlay()
{

	Super::BeginPlay();

	// Play built sound
	UGameplayStatics::SpawnSoundAtLocation(this, PlacementSound, GetActorLocation());

}

// Called every frame
void ABlockades::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}