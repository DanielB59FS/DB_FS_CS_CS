// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIBaseController.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSEIGE_API AAIBaseController : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* behaviorTree;
};
