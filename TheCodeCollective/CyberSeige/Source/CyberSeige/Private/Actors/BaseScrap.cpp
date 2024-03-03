// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BaseScrap.h"
#include "Actors/BasePlayer.h"
#include "Actors/ScrapPiece.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Actors/Interfaces/ScrapSpawnerInterface.h"

// Sets default values
ABaseScrap::ABaseScrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Particles"));
	CogMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cog Mesh"));

	// Setup Attachment
	RootComponent = DetectionSphere;
	NiagaraParticles->SetupAttachment(RootComponent);
	CogMesh->SetupAttachment(RootComponent);

	// Configure Components
	DetectionSphere->SetSphereRadius(64.0f);	
	DetectionSphere->SetCollisionProfileName(FName("IgnoreAll"));
	DetectionSphere->SetCollisionObjectType(ECC_PhysicsBody);
	DetectionSphere->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Overlap);
	CogMesh->SetCollisionProfileName(FName("NoCollision"));

	// Set default values
	CogRotSpeed = 0.5f;
	CogPosSpeed = 5.0f;
}

// Called when the game starts or when spawned
void ABaseScrap::BeginPlay()
{
	Super::BeginPlay();

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseScrap::Explode);
}

// Called every frame
void ABaseScrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Two separate variables so we can manipulate the two different speeds
	CogHoverRads += DeltaTime * CogPosSpeed;
	CogRotRads = DeltaTime * CogRotSpeed;

	// Can't directly modulus the values since they are floats
	if (CogHoverRads >= 6.28f) CogHoverRads = 0;

	// Make sure weapon mesh is not null
	if (CogMesh == nullptr) return;

	// Define our offset and multply by Cos, effectively getting an offset of -0.15 to 0.15
	FVector Offset = { 0, 0, 0.25f };
	FVector RotOffset = { 0, 0, CogRotRads};

	// Calulcate offset
	Offset *= cos(CogHoverRads);

	// Add ofset to the mesh
	CogMesh->AddWorldOffset(Offset);
	CogMesh->AddWorldRotation(FQuat::MakeFromEuler(RotOffset));
}

void ABaseScrap::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{

	// If we implement the interface
	if (OtherActor->GetClass()->ImplementsInterface(UScrapSpawnerInterface::StaticClass()))
	{
		ScrapTarget = OtherActor;

		GetWorldTimerManager().SetTimer(handle, this, &ABaseScrap::SpawnScrap, 0.025f, true, 0.0f);
		OnScrapCollected.Broadcast();
	}
}

void ABaseScrap::SpawnScrap()
{
	// Break the scrap disbursed into increments of 10
	if (ScrapAmount > 0)
	{
		FActorSpawnParameters SpawnParameters;
		FVector location = GetActorLocation() + FVector(0, 0, 25);
		AScrapPiece* NewPiece = GetWorld()->SpawnActor<AScrapPiece>(location, GetActorRotation());

		if (NewPiece == nullptr) return;

		NewPiece->SetHomingTarget(ScrapTarget);
		NewPiece->SetMesh(ScrapPieceMesh);

		if (ScrapAmount >= 10)
		{
			NewPiece->SetScrapValue(10);
			ScrapAmount -= 10;
		}
		else if (ScrapAmount > 0 && ScrapAmount < 10)
		{
			NewPiece->SetScrapValue(ScrapAmount);
			ScrapAmount -= ScrapAmount;
		}
	}

	if (ScrapAmount <= 0)
	{
		GetWorldTimerManager().ClearTimer(handle);

		if (GetAttachParentActor() != nullptr)
			GetAttachParentActor()->Destroy();

		Destroy();
	}
}

void ABaseScrap::SetScrapAmount(float NewAmount)
{
	ScrapAmount = NewAmount;
}

