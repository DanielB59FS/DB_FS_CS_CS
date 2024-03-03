// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/ExplosionAoe.h"
#include "Kismet/GameplayStatics.h"

AExplosionAoe::AExplosionAoe()
{
	Damage = 25.0f;
}

void AExplosionAoe::ApplyEffect(AActor* target)
{
	for (FName Tag : target->Tags)
	{
		if (Tag.Compare(FName("Enemy")) == 0)
		{
			UGameplayStatics::ApplyDamage(target, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		}
	}
}