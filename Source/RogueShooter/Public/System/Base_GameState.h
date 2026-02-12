// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Base_GameState.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ABase_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABase_GameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 플레이어 각자 정보를 가지고 있는게 좋지않나? 
	void IncreaseEnemyKill();
    
	// UI에 표시될 게임 시간 (서버->클라이언트 복제 필요)
	UPROPERTY(ReplicatedUsing = OnRep_GameTime, BlueprintReadOnly)
	FText GameTime;

	UFUNCTION()
	void OnRep_GameTime();

	// 킬 수 (점수)
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 EnemiesKilled = 0;

	// 전역 레벨 (모든 플레이어 공유라고 가정)
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 Level = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 CurrentXP = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 NeededXP = 0;

	// --- [Helper Functions] ---
	void UpdateGameTime(int32 Minutes, int32 Seconds);
};
