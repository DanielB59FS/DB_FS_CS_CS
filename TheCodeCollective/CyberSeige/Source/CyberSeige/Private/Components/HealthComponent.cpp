// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Current = Max;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleDamage);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Current = FMath::Clamp(Current - Damage, 0.f, Max);

	if (Current > 0)
	{
		if (Damage > 0)
		{
			OnDamaged.Broadcast(Current / Max);
			OnHealthChange.Broadcast(Current / Max);
		}
		else
		{
			OnHealed.Broadcast(Current / Max);
			OnHealthChange.Broadcast(Current / Max);
		}
	}
	else
	{
		GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UHealthComponent::HandleDamage);

		OnDeath.Broadcast(Current / Max);
		OnHealthChange.Broadcast(Current / Max);
	}
}
