// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Castle.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/WaveGameMode.h"

// Sets default values
ACastle::ACastle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ACastle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACastle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (healthComponent->Current <= 0)
	{		
		AWaveGameMode* waveGameMode = Cast<AWaveGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (nullptr != waveGameMode)
		{
			waveGameMode->OnGameEnded.Broadcast(FText::FromString("You Lost! \nCastle Destroyed"));
		}
	}
}

