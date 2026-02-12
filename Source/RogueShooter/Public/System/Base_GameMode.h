// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "Base_GameMode.generated.h"

class ABase_Enemy;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ABase_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABase_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// 초기화
	void GameSetup();
	void SpawnSetup();
    void BuildPlayerArray();
	
	// 스폰 관련
	void SpawnWave();
	void FindSpawnLocation();
    
	UFUNCTION() // EQS 델리게이트용
	void SpawnEnemy(class UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status);
    
	UFUNCTION()
	void SpawnElite(class UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status);
	
	
	// 게임 종료 및 상태 관리
	void EndGame(bool Victory);
	void DetermineGameStatus();
	void OnPlayerDeath(); // 기존 OnPlayerDeath_Implementation 대체
	void ProcessEndGame();
	
	UFUNCTION()
	void IncreaseEnemyKill();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
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
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> EnemySpawnDT;
    
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> EnemyEliteSpawnDT;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnvQuery> EQS_FindSpawnPoint;

	// 타이머 핸들
	FTimerHandle SpawnTimerReference;
	FTimerHandle PhaseIndexTimer;
	FTimerHandle ClockReference;

	// 내부 계산용 변수
	int32 Time = 0;
	int32 Minutes = 0;
	int32 CurrentEnemyCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Pause")
	bool GameIsPaused = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Pause")
	int32 PauseCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<ABase_Enemy> EnemyClass;
	
	
private:
	void UpdateTimer();
	void IncreaseWaveIndex();
    
	// 플레이어 캐싱 (GameMode는 PlayerArray를 GameState에서 접근 가능하지만 편의상 캐싱 가능)
	UPROPERTY()
	TArray<class ABase_Character*> CachedPlayerCharacterArray;
	
	UPROPERTY()
	TArray<class AGameplay_PlayerController*> PlayerControllerArray;
	
	UPROPERTY()
	int32 EnemiesKilled;
};
