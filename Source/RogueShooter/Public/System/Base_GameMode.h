// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Base_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ABase_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABase_GameMode();

	// Reset int tracking number of players who try to resume
	// 재개하려는 플레이어의 수를 초기화한다.(pause count)
	virtual void ResetPauseCount();

	// Check How many alive players there are against the number of people who have finished the level up or chest flow
	// 상자 열기나 레벨업을 끝낸 player의 수가 그렇지 않은 players(alive)보다 얼마나 많은지 체크한다.
	virtual bool CheckPlayersForPause();

	// Return number of alive players
	// 살아있는 player의 수를 반환한다.
	virtual int32 GetAlivePlayers();

	// Travel back to lobby
	// 로비로 되돌아오기 
	virtual void ServerTravel_Lobby();

	// Travel to map using seamless travel
	// seamless travel을 이용해 map으로 이동 
	virtual void ServerTravel_GamePlay(FName Map);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Pause")
	bool GameIsPaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Pause")
	int32 PauseCount = 0;
};
