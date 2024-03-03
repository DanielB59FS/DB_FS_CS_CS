// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/BaseThrowable.h"
#include "Actors/Weapons/BaseAoeEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseThrowable::ABaseThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));

	// Setup attachments
	RootComponent = Mesh;
	Sphere->SetupAttachment(Mesh);
	Trail->SetupAttachment(Mesh);

	// Change sphere raidus
	Sphere->SetSphereRadius(16.0f);

	// Define variables
	Delay = 3;
	ExplodeOnImpact = false;
}

// Called when the game starts or when spawned
void ABaseThrowable::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ExplodeOnImpact)
	{
		// Start timer if the throwable doesn't explode on impact
		FTimerHandle tempHandle;
		GetWorldTimerManager().SetTimer(tempHandle, this, &ABaseThrowable::SpawnEffectNoArgs, Delay, false, 0.0f);
	}
	else
	{
		// Bind OnComponentBeginOverlap if it explodes on impact
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseThrowable::SpawnEffect);
	}
}

// helper function to bind to the timer
void ABaseThrowable::SpawnEffectNoArgs()
{
	FHitResult tempResult;
	SpawnEffect(nullptr, nullptr, nullptr, -1, false, tempResult);
}

// Spawns the specified effect at the location of the throwable
void ABaseThrowable::SpawnEffect(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult) 
{
	FActorSpawnParameters SpawnInfo;

	GetWorld()->SpawnActor<ABaseAoeEffect>(Mesh->GetComponentLocation(), FRotator(0, 0, 0), SpawnInfo);

	Destroy();
}

