// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/AIBaseController.h"
#include "BehaviorTree/BehaviorTree.h"

void AAIBaseController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(behaviorTree);
}