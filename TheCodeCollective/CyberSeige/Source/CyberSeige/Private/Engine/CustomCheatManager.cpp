// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/CustomCheatManager.h"

#include "CyberSeige/CyberSeige.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/CustomCheatExtension.h"
#include "Actors/BasePlayer.h"
#include "HUD/GameplayHUD.h"
#include "GameModes/WaveGameMode.h"

UCustomCheatManager::UCustomCheatManager() {
	//for (auto extension : extensions)
	//	AddCheatManagerExtension(extension.GetDefaultObject());
	//	//AddCheatManagerExtension((UCheatManagerExtension*)CreateDefaultSubobject("extension", UCheatManagerExtension::StaticClass(), extension, false, false));
	UE_LOG(Game, Warning, TEXT("Cheat Mannager! - Default"));
}

void UCustomCheatManager::CPrintMessage(const FString message) {
	GetOuterAPlayerController()->ClientMessage(message);
}

void UCustomCheatManager::CGod_Implementation() {
	God();
	//UE_LOG(Game, Error, TEXT("DERP!!!"));
	//ABasePlayer* player = Cast<ABasePlayer>(GetOuterAPlayerController()->GetCharacter());
	//if (false) {
	//	UPlayerHUD* hud = player->GetHUD();
	//	if (hud) {
	//		hud->SetHealth(1.f);
	//		hud->SetCastleHealth(1.f);
	//	}
	//
	//	//	TODO: Temporary solution:
	//	TArray<AActor*> actors;
	//	UGameplayStatics::GetAllActorsWithTag(player->GetWorld(), "castle", actors);
	//	AActor* castle = actors[0];
	//	castle->SetCanBeDamaged(player->CanBeDamaged());
	//}
}

void UCustomCheatManager::CAmmo_Implementation() { }

void UCustomCheatManager::CSpawn_Implementation(int spawnType) { }

void UCustomCheatManager::CSetScraps_Implementation(int count) { }

void UCustomCheatManager::CSelectNextWave_Implementation(int wave) {
	//Cast<AWaveGameMode>(UGameplayStatics::GetGameMode());
}

void UCustomCheatManager::CLoadMap_Implementation(int MapIndex) { }

void UCustomCheatManager::AnotherTest() {
	UE_LOG(Game, Warning, TEXT("Cheat Manager - method!"));
}
