// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Motherboard.h"
#include "Baseboard.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CYBERSEIGE_API UBaseboard : public UActorComponent {
	GENERATED_BODY()

private:
	bool bOverclocked;

	std::priority_queue<class IChip*> Equipped;
	TArray<class IChip*> Chips;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UBaseboard();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
