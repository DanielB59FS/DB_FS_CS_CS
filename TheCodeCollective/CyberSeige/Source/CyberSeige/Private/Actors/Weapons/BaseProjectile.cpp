// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "..\..\..\CyberSeige.h"
#include "Actors/Weapons/BaseAoeEffect.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collisio0n"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));

	// Setup attachments
	RootComponent = Collision;
	Mesh->SetupAttachment(RootComponent);
	Particles->SetupAttachment(Mesh);

	// Set default values for components
	ProjectileMovement->ProjectileGravityScale = 0.15f;
	ProjectileMovement->InitialSpeed = 750.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.5f;

	Particles->bAutoActivate = true;

	Collision->SetCollisionProfileName(FName("BlockAll"));
	Collision->SetGenerateOverlapEvents(false);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Collision->SetSimulatePhysics(false);

	// Set default values for variables
	SpawnsAoe = true;
	ActivatesOnImpact = true;
	DelayAmount = 1.5f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap event for collision
	Collision->OnComponentHit.AddDynamic(this, &ABaseProjectile::HandleImpact);
}

// Sets up a timer if the projectile doesn't explode on impact
void ABaseProjectile::SetupTimer()
{
	// Set a timer so the projectile can move before exploding. Can be used for launched grenades or something else.
	GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &ABaseProjectile::SpawnAoe, DelayAmount, false, 0.0f);
}

// Spawns the AOE if the projectile is meant to spawn one
void ABaseProjectile::SpawnAoe()
{
	// Spawn AOE actor and then destroy the projectile
	FActorSpawnParameters params;
	GetWorld()->SpawnActor<ABaseAoeEffect>(AoeEffect, GetActorLocation(), GetActorRotation(), params);

	Destroy();
}

// Helper function for binding to the Overlap event.
void ABaseProjectile::HandleImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector Impulse, const FHitResult& Hit)
{
	UE_LOG(Game, Warning, TEXT("Impact"));

	if (ActivatesOnImpact)
		SpawnAoe();
	else
		SetupTimer();
}

