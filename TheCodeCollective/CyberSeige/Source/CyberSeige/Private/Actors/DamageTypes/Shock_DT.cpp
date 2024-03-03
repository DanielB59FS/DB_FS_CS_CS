// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DamageTypes/Shock_DT.h"
#include "Kismet/GameplayStatics.h"
#include "Components/EffectComponent.h"
#include "Enums/Enums.h"

void UShock_DT::StartEffect(AActor* DamagedActor, AActor* DamageCauser, float Damage) const
{
	UEffectComponent* EffectComponent = DamagedActor->FindComponentByClass<UEffectComponent>();

	if (EffectComponent != nullptr)
	{
		EffectComponent->StartEffect(this, EEffects::Shocking, DamageCauser);
	}
}