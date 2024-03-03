// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ScrapPiece.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AScrapPiece::AScrapPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	PieceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

	// Setup component attachments
	RootComponent = Collision;
	PieceMesh->SetupAttachment(RootComponent);

	// Configure components
	InitialLifeSpan = 7.5f;

	Collision->SetSphereRadius(16.0f);
	Collision->SetCollisionProfileName(FName("IgnoreAll"));
	Collision->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Overlap);

	PieceMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Movement->InitialSpeed = 10000.0f;
	Movement->MaxSpeed = 1500.0f;
	Movement->bRotationFollowsVelocity = true;
	Movement->bIsHomingProjectile = false;
	Movement->HomingAccelerationMagnitude = 10000.0f;
	Movement->ProjectileGravityScale = 0.0f;
	Movement->Velocity = FVector(FMath::FRandRange(-0.5f, 0.5f), FMath::FRandRange(-0.5f, 0.5f), 1.0f);
}

// Called when the game starts or when spawned
void AScrapPiece::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AScrapPiece::SetHomingTrue, 5.0f, false, 0.0f);
}


void AScrapPiece::SetHomingTrue()
{
	Movement->bIsHomingProjectile = true;
}

void AScrapPiece::SetMesh(UStaticMesh* NewMesh)
{
	if (NewMesh == nullptr) return;
	PieceMesh->SetStaticMesh(NewMesh);
}

void AScrapPiece::SetHomingTarget(AActor* NewTarget)
{
	if (NewTarget == nullptr || NewTarget->GetRootComponent() == nullptr) return;

	Movement->HomingTargetComponent = NewTarget->GetRootComponent();
}

int AScrapPiece::GetScrapValue()
{
	return ScrapValue;
}


void AScrapPiece::SetScrapValue(int NewScrapValue)
{
	ScrapValue = NewScrapValue;
}
