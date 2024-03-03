// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestActivatorComponent.h"
#include "Interfaces/QuestActivatorInterface.h"
#include "..\..\CyberSeige.h"
#include <Kismet/GameplayStatics.h>
#include "Turrets.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UQuestActivatorComponent::UQuestActivatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	BoundStep = -1;
}


// Called when the game starts
void UQuestActivatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// Automatically add tag to owner so the quest manager picks it up
	// If the value isn't -1, that means we are trying to dynamically bind the activator
	if (BoundStep != -1)
		GetOwner()->Tags.Add(FName("UnboundActivator"));

	ActivateFunctions[ROTATE_OBJECT_X] = &UQuestActivatorComponent::RotateObjectX;
	ActivateFunctions[ROTATE_OBJECT_Y] = &UQuestActivatorComponent::RotateObjectY;
	ActivateFunctions[ROTATE_OBJECT_Z] = &UQuestActivatorComponent::RotateObjectZ;
	ActivateFunctions[START_WAVE_TIMER] = &UQuestActivatorComponent::StartWaveTimer;
	ActivateFunctions[ENABLE_SNIPER_TURRET] = &UQuestActivatorComponent::EnableSniperTurret;
	ActivateFunctions[ENABLE_LASER_FENCE] = &UQuestActivatorComponent::EnableLaserFence;
	ActivateFunctions[ENABLE_TELEPORTER] = &UQuestActivatorComponent::EnableTeleporter;
	ActivateFunctions[GIVE_SCRAP] = &UQuestActivatorComponent::GiveScrap;
	ActivateFunctions[DAMAGE_TURRETS] = &UQuestActivatorComponent::DamageTurrets;
	ActivateFunctions[DESTROY_ACTORS] = &UQuestActivatorComponent::DestroySpecifiedActors;
}


// Called every frame
void UQuestActivatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bRotate)
	{
		RotateObject(RotateSpeed * DeltaTime);
	}
}

// Quest activator helpers functions
void UQuestActivatorComponent::RotateObjectX()
{
	FRotator Rotation = GetOwner()->GetActorRotation();

	TargetRotation = FRotator(Rotation.Roll + DegreesToRotate, Rotation.Pitch, Rotation.Yaw);

	bRotate = true;
}

void UQuestActivatorComponent::RotateObjectY()
{
	FRotator Rotation = GetOwner()->GetActorRotation();

	TargetRotation = FRotator(Rotation.Roll, Rotation.Pitch + DegreesToRotate, Rotation.Yaw);

	bRotate = true;
}

void UQuestActivatorComponent::RotateObjectZ()
{
	FRotator Rotation = GetOwner()->GetActorRotation();

	TargetRotation = FRotator(Rotation.Roll, Rotation.Pitch, Rotation.Yaw + DegreesToRotate);

	bRotate = true;
}

void UQuestActivatorComponent::RotateObject(float Amount)
{
	GetOwner()->SetActorRotation(UKismetMathLibrary::RLerp(GetOwner()->GetActorRotation(), TargetRotation, Amount, false));
}

void UQuestActivatorComponent::StartWaveTimer()
{
	OnStartWaveTimer.Broadcast();
}

void UQuestActivatorComponent::EnableSniperTurret()
{
	OnEnableMenuItem.Broadcast(EDefenseType::Turret, 2);
}

void UQuestActivatorComponent::EnableLaserFence()
{
	OnEnableMenuItem.Broadcast(EDefenseType::Blockade, 1);
}

void UQuestActivatorComponent::EnableTeleporter()
{
	OnEnableMenuItem.Broadcast(EDefenseType::Blockade, 2);
}

void UQuestActivatorComponent::GiveScrap()
{
	OnGiveScrap.Broadcast(ScrapToGive);
}

void UQuestActivatorComponent::DamageTurrets()
{
	OnDamageTurrets.Broadcast();
}

void UQuestActivatorComponent::DestroySpecifiedActors()
{
	TArray<AActor*> ActorsOfClass;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorsToDestroy, ActorsOfClass);

	for (AActor* Actor : ActorsOfClass)
	{
		Actor->Destroy();
	}
}

// Begin QuestActivatorInterface
void UQuestActivatorComponent::StartActivate_Implementation()
{
	if (ActivateFunctions[BehaviorIndex] != nullptr)
	{
		(this->* (ActivateFunctions[BehaviorIndex]))();
		UE_LOG(Game, Log, TEXT("Activated Function"))
	}
	else
		UE_LOG(Game, Error, TEXT("Invalid Behavior Index"))
}

int UQuestActivatorComponent::GetBoundStep_Implementation()
{
	return BoundStep;
}
// End QuestActivatorInterface

