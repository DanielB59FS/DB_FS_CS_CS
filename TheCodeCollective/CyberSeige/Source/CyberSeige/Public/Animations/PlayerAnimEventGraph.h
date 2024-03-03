// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/Enums.h"
#include "PlayerAnimEventGraph.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootAnimationEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadAnimationEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowableAnimationEnded);

UCLASS()
class CYBERSEIGE_API UPlayerAnimEventGraph : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeUpdateAnimation(float deltaSeconds) override;

	// Functions for playing animations in slots
	UFUNCTION(BlueprintImplementableEvent, Category = "Event Graph | Fucntions")
		void PlayActionAnimation(UAnimSequenceBase* ActionAnimation);

	UFUNCTION(BlueprintImplementableEvent, Category = "Event Graph | Fucntions")
		void PlayShootAnimation(UAnimSequenceBase* ShootAnimation);

	// Class Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Event Graph | Event Delegates")
		FOnShootAnimationEnded OnShootAnimationEnded;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Event Graph | Event Delegates")
		FOnReloadAnimationEnded OnReloadAnimationEnded;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Event Graph | Event Delegates")
		FOnThrowableAnimationEnded OnThrowableAnimationEnded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Event Graph | Variables")
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Event Graph | Variables")
		float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Event Graph | Variables")
		TEnumAsByte<WeaponType> WeaponTypeEnum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Event Graph | Variables")
		TEnumAsByte<PlayerAnimationState> PlayerState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Event Graph | Variables")
		FName ActionSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Event Graph | Variables")
		FName ShootSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Event Graph | Variables")
		FName LeftHandPosSocket;

private:
	UPROPERTY(VisibleAnywhere, Category = "Event Graph | Variables")
		FVector LeftHandPos;
};
