// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/BaseAoeEffect.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseAoeEffect::ABaseAoeEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));

	// Setup attachments
	RootComponent = Collision;
	Particles->SetupAttachment(RootComponent);

	// Configure components
	Collision->SetCollisionProfileName(FName("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ABaseAoeEffect::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind destroy to when the partcle system finishes playing
	if (!Particles->IsPendingKill())
		Particles->OnSystemFinished.AddDynamic(this, &ABaseAoeEffect::DestroyAoe);

	// Get overlapping actors
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1) };
	const TArray<AActor*> IgnoreList = {};
	TArray<AActor*> OverlappingActors = {};

	UKismetSystemLibrary::ComponentOverlapActors(Collision, Collision->GetComponentTransform(), ObjectTypes, ACharacter::StaticClass(), IgnoreList, OverlappingActors);

	// Play the AOE sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, Collision->GetComponentLocation());

	// Apply effects on overlapped actors
	for (AActor* Actor : OverlappingActors)
	{
		ApplyEffect(Actor);
	}

	// Disable Collisions on sphere component after we get the current affected actors
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ABaseAoeEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseAoeEffect::DestroyAoe(UParticleSystemComponent* ParticlesToDestroy)
{
	Destroy();
}

