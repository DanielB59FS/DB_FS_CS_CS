// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ShockEffectActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../CyberSeige.h"
#include "Actors/DamageTypes/Shock_DT.h"
#include "GameFramework/Character.h"

// Sets default values
AShockEffectActor::AShockEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create Components
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	NiagaraParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticles"));

	// Setup Attachments
	RootComponent = Collision;
	NiagaraParticles->SetupAttachment(Collision);

	// Configure default values
	SpreadCharges = 3;
	Collision->SetSphereRadius(300.0f);
	Collision->SetGenerateOverlapEvents(true);
	Collision->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

// Called every frame
void AShockEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AShockEffectActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(Game, Warning, TEXT("Created ShockEffectActor in %s"), *GetOwner()->GetName());

	ShockedList.Add(this);

	SetShockParameters();
}


void AShockEffectActor::SetShockParameters(int newSpreadCharges, int newShockCharges, float newShockInterval, float newShockDamage)
{
	SpreadCharges = newSpreadCharges;
	ShockCharges = newShockCharges;
	ShockInterval = newShockInterval;
	ShockDamage = newShockDamage;
	ShockedList.Add(this);

	UE_LOG(Game, Warning, TEXT("%s: Started shock with %i charges"), *GetOwner()->GetName(), ShockCharges);
	GetWorldTimerManager().SetTimer(ShockTimerHandle, this, &AShockEffectActor::ApplyShock, ShockInterval, true, 0.25f);
}

void AShockEffectActor::ApplyShock()
{
	// Subract Shock Charge
	ShockCharges--;

	// Apply Damage
	UE_LOG(Game, Warning, TEXT("%s: Applied Shock Damage"), *GetOwner()->GetName());
	UGameplayStatics::ApplyDamage(GetParentActor(), ShockDamage, GetInstigatorController(), this, UDamageType::StaticClass());

	// Search for other enemies
	TSet<AActor*> OverlappingActors;

	Collision->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

	// Apply shock to other actors in the radius
	if (OverlappingActors.Num() >= 1 && SpreadCharges >= 1)
	{
		for (auto& Actor : OverlappingActors)
		{
			if (!ShockedList.Contains(Actor) && Actor != this && Actor != GetOwner())
			{
				// Subtract Spread Charge
				SpreadCharges--;

				// Spread shock effect
				UE_LOG(Game, Warning, TEXT("%s: Spread Shock Damage to %s"), *GetOwner()->GetName(), *Actor->GetName());
				ShockedList.Add(Actor);
				UGameplayStatics::ApplyDamage(Actor, 0.1f, GetInstigatorController(), this, UShock_DT::StaticClass());
			}
			else
				UE_LOG(Game, Warning, TEXT("%s is already being shocked!"), *Actor->GetName());
		}
	}
	
	if (ShockCharges <= 0)
	{
		GetWorldTimerManager().ClearTimer(ShockTimerHandle);
		OnShockActorEnded.Broadcast();
		Destroy();
	}

}

void AShockEffectActor::SetShockedList(TSet<AActor*> NewList)
{
	if (NewList.Num() == 0) return;

	ShockedList = NewList;
}

TSet<AActor*> AShockEffectActor::GetShockedList()
{
	return ShockedList;
}