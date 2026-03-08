// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueShooter/RSEnumStruct.h"
#include "WaveManagerComponent.generated.h"


// 웨이브 상태 Enum
UENUM(BlueprintType)
enum class EWaveState : uint8
{
	NotStarted,
	Preparing,
	Spawning,
	Combat,
	WaveCleared,
	GameOver
};

// UI 업데이트용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveStateChanged, EWaveState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, CurrentCount);

// 내부에서 각 몬스터 그룹의 스폰 상태를 추적하는 구조체
USTRUCT()
struct FSpawnTask
{
	GENERATED_BODY()

	TSubclassOf<class ABase_Enemy> MonsterClass;
	int32 TargetCount = 0;
	int32 SpawnedCount = 0;
	FTimerHandle TimerHandle;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API UWaveManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWaveManagerComponent();

	virtual void BeginPlay() override;

	// 블루프린트나 게임 모드에서 호출하여 웨이브를 시작하는 함수
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartWave(int32 WaveIndex);

	// 몬스터가 죽었을 때 호출할 함수 (몬스터 클래스에서 델리게이트로 바인딩 추천)
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void OnMonsterDied(AActor* DeadMonster);

	// 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Wave|Events")
	FOnWaveStateChanged OnWaveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave|Events")
	FOnEnemyCountChanged OnEnemyCountChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Wave|Data")
	UDataTable* WaveDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|State")
	EWaveState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|State")
	int32 CurrentWaveIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave|State")
	int32 AliveEnemyCount;

	UPROPERTY()
	TArray<AActor*> SpawnPoints;
private:
	// 스폰 준비가 끝나고 본격적으로 타이머를 돌리는 함수
	void BeginSpawning(const FWaveInfo* WaveInfo);

	// 타이머에 의해 반복 호출되는 실제 스폰 로직 (TaskIndex를 페이로드로 받음)
	void SpawnMonsterRoutine(int32 TaskIndex);

	// 상태 변경 및 델리게이트 브로드캐스트를 통합 처리하는 헬퍼 함수
	void SetWaveState(EWaveState NewState);

	// 웨이브 클리어 조건 검사
	void CheckWaveClearCondition();

	// 진행 중인 스폰 태스크 배열
	UPROPERTY()
	TArray<FSpawnTask> ActiveSpawnTasks;

	FTimerHandle PrepareTimerHandle;
};
