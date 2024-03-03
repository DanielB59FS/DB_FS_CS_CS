// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateOneParam, float, HealthRatio);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBERSEIGE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		float Max = 100;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Current;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CodeDispatcher")
		FDelegateOneParam OnDamaged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CodeDispatcher")
		FDelegateOneParam OnDeath;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CodeDispatcher")
		FDelegateOneParam OnHealed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CodeDispatcher")
		FDelegateOneParam OnHealthChange;

	UFUNCTION()
		void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
