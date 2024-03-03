// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "CustomCheatManager.generated.h"

/**
 *
 */
UCLASS()
class CYBERSEIGE_API UCustomCheatManager : public UCheatManager {
	GENERATED_BODY()

private:
	//UPROPERTY(EditDefaultsOnly)
	//TArray<TSubclassOf<UCheatManagerExtension>> extensions;

public:
	UCustomCheatManager();

	UFUNCTION(BlueprintCallable)
	void CPrintMessage(const FString message);

	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CGod();

	virtual void CGod_Implementation();
	
	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CAmmo();

	virtual void CAmmo_Implementation();
	
	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CSpawn(int spawnType);

	virtual void CSpawn_Implementation(int spawnType);
	
	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CSetScraps(int spawnType);

	virtual void CSetScraps_Implementation(int count);
	
	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CSelectNextWave(int wave);

	virtual void CSelectNextWave_Implementation(int wave);
	
	UFUNCTION(Exec, BlueprintNativeEvent, BlueprintCallable)
	void CLoadMap(int MapIndex);

	virtual void CLoadMap_Implementation(int MapIndex);

	UFUNCTION(Exec, BlueprintCallable)
	void AnotherTest();
};
