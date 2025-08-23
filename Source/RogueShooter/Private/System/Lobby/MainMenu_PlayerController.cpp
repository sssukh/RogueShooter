// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/MainMenu_PlayerController.h"

#include "RogueShooter/AssetPath.h"
#include "UI/UW_MainMenu.h"

AMainMenu_PlayerController::AMainMenu_PlayerController()
{
	static ConstructorHelpers::FClassFinder<UUW_MainMenu> MainMenuFinder(*AssetPath::Blueprint::WBP_MainMenu_C);

	if(MainMenuFinder.Succeeded())
		MainMenuClass = MainMenuFinder.Class;
}

void AMainMenu_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(!IsLocalPlayerController())
		return;

	UUW_MainMenu* MainMenu = CreateWidget<UUW_MainMenu>(this,MainMenuClass);

	MainMenu->AddToViewport();

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(MainMenu->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputModeUIOnly);
	
}
