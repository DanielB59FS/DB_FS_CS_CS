// Fill out your copyright notice in the Description page of Project Settings.


#include "Defenses.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"

// Sets default values
ADefenses::ADefenses()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void ADefenses::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ADefenses::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

