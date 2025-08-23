// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/Lobby_GameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_CharacterManager.h"
#include "Interface/Interface_ControllerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/RSLog.h"

ALobby_GameMode::ALobby_GameMode()
{
}

void ALobby_GameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([&]()
	{
		UpdateLobbyUI();

		UpdateClassNames();
	}),1.0f,false);
}

void ALobby_GameMode::UpdateLobbyUI()
{
	TArray<APlayerState*> PlayerStates = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;

	for(APlayerState* PS : PlayerStates)
	{
		if(PS->GetPlayerController()->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
		{
			IInterface_ControllerManager::Execute_OnPlayerJoin(PS->GetPlayerController());
		}
		else
		{
			RS_LOG_ERROR(TEXT("Controller가 ControllerManager 인터페이스를 상속받지 않았습니다."))
		}
	}
}

void ALobby_GameMode::UpdateClassNames()
{
	TArray<APlayerState*> PlayerStates = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;

	for(APlayerState* PS : PlayerStates)
	{
		if(PS->GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			IInterface_CharacterManager::Execute_UpdateClassName(PS->GetPawn());
		}
		else
		{
			RS_LOG_ERROR(TEXT("Pawn이 CharacterManager 인터페이스를 상속받지 않았습니다."))
		}
	}
}
