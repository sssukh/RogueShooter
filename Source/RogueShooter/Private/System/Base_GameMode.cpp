// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueShooter/Public/System/Base_GameMode.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Interface/Interface_CharacterManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ABase_GameMode::ABase_GameMode()
{
}


void ABase_GameMode::ResetPauseCount()
{
	PauseCount = 0;
}

bool ABase_GameMode::CheckPlayersForPause()
{
	if(GameIsPaused)
	{
		int32 playersCount = GetAlivePlayers();

		if(++PauseCount >= playersCount)
		{
			PauseCount = 0;

			return true;
		}

	}
	return false;
}

int32 ABase_GameMode::GetAlivePlayers()
{
	AGameStateBase* GameStateBase = GetGameState<AGameStateBase>();

	int32 alivePlayers = 0;

	// playerstate 순회하며 살아있는 플레이어의 수 얻기 
	for(const APlayerState* ps : GameStateBase->PlayerArray)
	{
		if(IInterface_CharacterManager::Execute_IsAlive(ps->GetPawn()))
		{
			++alivePlayers;
		}
	}
	return alivePlayers;
}

// TODO command
void ABase_GameMode::ServerTravel_Lobby()
{
	FString cmd = "/Game/SurvivorsTemplate/Maps/M_Lobby";
	
	GetWorld()->ServerTravel(cmd);
	
}

// TODO command
void ABase_GameMode::ServerTravel_GamePlay(FName Map)
{
	FString cmd = FString::Printf(TEXT("/Game/SurvivorsTemplate/Maps/%s"),*Map.ToString());

	GetWorld()->ServerTravel(cmd);
}
