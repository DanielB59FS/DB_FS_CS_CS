// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/CustomCheatExtension.h"

#include "CyberSeige/CyberSeige.h"

UCustomCheatExtension::UCustomCheatExtension() {
	UE_LOG(Game, Warning, TEXT("Cheat extention!"));
	//IConsoleManager::Get().RegisterConsoleCommand(TEXT("MyTest"), TEXT("Derp"));
}

void UCustomCheatExtension::MyTest(float val) {
	UE_LOG(Game, Warning, TEXT("Test Method!"));
}
