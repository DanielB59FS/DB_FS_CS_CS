// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EffectComponent.h"
#include "Enums/Enums.h"
#include "Actors/DamageTypes/Shock_DT.h"
#include "Actors/ShockEffectActor.h"
#include "../../CyberSeige.h"

// Sets default values for this component's properties
UEffectComponent::UEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// this function is meant to be an all purpose "effect" starter, but there was a lot of issues in passing
// the proper parameters, so I just made it for shock only
void UEffectComponent::StartEffect(const UShock_DT* DamageType, EEffects DamageEffect, AActor* DamageCauser)
{
	switch (DamageEffect)
	{
	case Shocking:
		StartShock(DamageCauser);

		break;
	default:
		break;
	}
}

void UEffectComponent::StartShock(AActor* DamageCauser)
{
	FActorSpawnParameters params;
	params.Owner = GetOwner();

	UE_LOG(Game, Warning, TEXT("Shock damage started by %s"), *DamageCauser->GetName())

	AShockEffectActor* ShockEffectActor = GetWorld()->SpawnActor<AShockEffectActor>(BPShockEffect, GetComponentLocation(), GetComponentRotation(), params);
	AShockEffectActor* Parent = Cast<AShockEffectActor>(DamageCauser);

	// If the damage causer was a shock effect, get the parameters and set it
	if (Parent != nullptr)
	{
		UE_LOG(Game, Warning, TEXT("%s: Spread to %s with %i charges"), *DamageCauser->GetName(), *GetOwner()->GetName(), Parent->SpreadCharges);
		ShockEffectActor->SetShockedList(Parent->GetShockedList());
		ShockEffectActor->SetShockParameters(Parent->SpreadCharges - 1,  Parent->ShockCharges + 1, Parent->ShockInterval, Parent->ShockDamage);
	}

	ShockEffectActor->OnShockActorEnded.AddDynamic(this, &UEffectComponent::EndShock);

	if (ShockEffectActor != nullptr)
	{
		FAttachmentTransformRules rules = { EAttachmentRule::KeepWorld, true };

		ShockEffectActor->AttachToActor(GetOwner(), rules);
	}
}

void UEffectComponent::EndShock()
{
	OnShockEnded.Broadcast();
}

