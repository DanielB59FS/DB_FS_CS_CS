// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WeaponDispenser.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Actors/Interfaces/ScrapSpawnerInterface.h"
#include "Components/TextRenderComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponDispenser::AWeaponDispenser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Components
	DispenserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dispenser Mesh"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	PlayerDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetection"));
	TextRenderer = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Renderer"));

	// Setup attachments
	SetRootComponent(DispenserMesh);
	WeaponMesh->SetupAttachment(RootComponent);
	Collision->SetupAttachment(RootComponent);
	PlayerDetection->SetupAttachment(RootComponent);
	TextRenderer->SetupAttachment(RootComponent);

	// Set Default Values
	RotationSpeed = 0.5f;
	HoverSpeed = 5.0f;
	TextRenderer->SetVisibility(false);
	PlayerDetection->AreaClass = nullptr;
	Collision->AreaClass = nullptr;
}

// Called when the game starts or when spawned
void AWeaponDispenser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponDispenser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Two separate variables so we can manipulate the two different speeds
	WeaponHoverRads += DeltaTime * HoverSpeed;
	WeaponRotateDegrees += DeltaTime * RotationSpeed;

	// Can't directly modulus the values since they are floats
	if (WeaponHoverRads >= 6.28f) WeaponHoverRads = 0;
	if (WeaponRotateDegrees >= 360.0f) WeaponRotateDegrees = 0;

	// Make sure weapon mesh is not null
	if (WeaponMesh == nullptr) return;

	// Define our offset and multply by Cos, effectively getting an offset of -0.15 to 0.15
	FVector Offset = { 0, 0, 0.25f };
	FVector RotOffset = { 0, 0, WeaponRotateDegrees };

	// Calulcate offset
	Offset *= cos(WeaponHoverRads);

	// Add ofset to the mesh
	WeaponMesh->AddWorldOffset(Offset);
	WeaponMesh->SetWorldRotation(FQuat::MakeFromEuler(RotOffset));

	DisplayText();
}

// Check if the player is looking at the dispenser
void AWeaponDispenser::DisplayText()
{
	// Check if the player is looking at the dispenser
	TArray<AActor*> Overlaps;
	PlayerDetection->GetOverlappingActors(Overlaps);

	if (Overlaps.Num() > 0)
	{
		for (AActor* Actor : Overlaps)
		{
			// Only the player should be implementing this interface
			if (Actor->GetClass()->ImplementsInterface(UScrapSpawnerInterface::StaticClass()))
			{
				APawn* Pawn = Cast<APawn>(Actor);

				if (Pawn != nullptr)
				{
					FVector StartPos = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();;
					FVector EndPos = (StartPos + (UKismetMathLibrary::GetForwardVector(Pawn->GetBaseAimRotation()) * 500));

					TArray<FHitResult> OutHits;
					GetWorld()->LineTraceMultiByChannel(OutHits, StartPos, EndPos, ECC_PhysicsBody);

					// Check if one of the hits is this actor
					for (const FHitResult Result : OutHits)
					{
						if (Result.Actor == this && Result.Component == DispenserMesh)
						{
							TextRenderer->SetVisibility(true);
							bTargetedByPlayer = true;
							break;
						}
						else
						{
							TextRenderer->SetVisibility(false);
							bTargetedByPlayer = false;
						}
					}
				}
				else
				{
					TextRenderer->SetVisibility(false);
					bTargetedByPlayer = false;
				}
			}
			else
			{
				TextRenderer->SetVisibility(false);
				bTargetedByPlayer = false;
			}
		}
	}
	else
	{
		TextRenderer->SetVisibility(false);
		bTargetedByPlayer = false;
	}

}
