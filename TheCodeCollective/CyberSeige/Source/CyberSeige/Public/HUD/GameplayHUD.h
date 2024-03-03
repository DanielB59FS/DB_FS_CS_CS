// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayHUD.generated.h"

/**
 *
 */
UCLASS()
class CYBERSEIGE_API UGameplayHUD : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealth(float Ratio);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCastleHealth(float Ratio);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetReticleColor(FLinearColor Color);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAmmo(float Current, float Max);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetScraps(int count);
};
