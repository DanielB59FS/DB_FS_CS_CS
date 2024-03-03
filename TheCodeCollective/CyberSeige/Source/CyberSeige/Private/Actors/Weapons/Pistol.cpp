// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Pistol.h"

void APistol::Fire(APawn* PlayerOwner)
{
	FHitResult outHit;

	bool didHit = FireHitscan(PlayerOwner, outHit);
}

