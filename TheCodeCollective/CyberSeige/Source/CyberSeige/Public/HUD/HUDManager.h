// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "HUDManager.generated.h"

/**
 *
 */
UCLASS()
class CYBERSEIGE_API AHUDManager : public AHUD {
	GENERATED_BODY()

public:
	AHUDManager();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	class UGameplayHUD* GetGameplayHUD();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	class UMinimapHUD* GetMinimapHUD();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	UUserWidget* GetBuildHUD();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetVisibility(ESlateVisibility InVisibility);

	void SetVisibility_Implementation(ESlateVisibility InVisibility);
};
