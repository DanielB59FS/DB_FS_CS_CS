// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDManager.h"

#include "HUD/GameplayHUD.h"
#include "HUD/MinimapHUD.h"

AHUDManager::AHUDManager() : Super() { }

void AHUDManager::SetVisibility_Implementation(ESlateVisibility InVisibility) {
	GetGameplayHUD()->SetVisibility(InVisibility);
	GetMinimapHUD()->SetVisibility(InVisibility);
	GetBuildHUD()->SetVisibility(InVisibility);
}
