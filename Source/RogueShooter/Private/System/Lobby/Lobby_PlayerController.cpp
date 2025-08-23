// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/Lobby_PlayerController.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_LobbyMenu.h"

ALobby_PlayerController::ALobby_PlayerController()
{
	static ConstructorHelpers::FClassFinder<UUW_LobbyMenu> WidgetClassFinder(*AssetPath::Blueprint::WBP_LobbyMenu_C);

	if(WidgetClassFinder.Succeeded())
		LobbyMenuClass = WidgetClassFinder.Class;
}

void ALobby_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocalPlayerController())
	{
		CleanUpUI();

		CreateUI();
	}
}

void ALobby_PlayerController::OnPlayerJoin_Implementation()
{
	IInterface_ControllerManager::OnPlayerJoin_Implementation();

	OC_UpdatePlayerTotal();
}

void ALobby_PlayerController::OnMatchEnd_Implementation(bool Victory, int32 EnemiesKilled)
{
	IInterface_ControllerManager::OnMatchEnd_Implementation(Victory, EnemiesKilled);
}

void ALobby_PlayerController::UpdateLevelBar_Implementation(float Percent, int32 Level)
{
	IInterface_ControllerManager::UpdateLevelBar_Implementation(Percent, Level);
}

void ALobby_PlayerController::OC_UpdatePlayerTotal_Implementation()
{
	PlayerHud->UpdatePlayerTotal(UGameplayStatics::GetGameState(GetWorld())->PlayerArray.Num());
}

void ALobby_PlayerController::CreateUI()
{
	if(IsValid(PlayerHud))
	{
		if(PlayerHud->IsInViewport())
			return;
		
	}
	else
	{
		PlayerHud = CreateWidget<UUW_LobbyMenu>(this,LobbyMenuClass);
	}

	PlayerHud->AddToViewport();

	FInputModeGameAndUI InputModeGameAndUI;
	InputModeGameAndUI.SetWidgetToFocus(PlayerHud->TakeWidget());
	InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeGameAndUI.SetHideCursorDuringCapture(false);
	
	SetInputMode(InputModeGameAndUI);
}
