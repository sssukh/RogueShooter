// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WaveManagerComponent.h"

#include "Components/MonsterPoolComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UWaveManagerComponent::UWaveManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Tick 연산 완전 비활성화 (최적화)
	CurrentState = EWaveState::NotStarted;
	CurrentWaveIndex = 0;
	AliveEnemyCount = 0;
}


// Called when the game starts
void UWaveManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// 시작 시점에 "MonsterSpawner" 태그를 가진 모든 액터를 찾아 배열에 저장 (최적화)
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MonsterSpawner"), SpawnPoints);

	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("맵에 MonsterSpawner 태그를 가진 액터가 없습니다!"));
	}
	
}

void UWaveManagerComponent::StartWave(int32 WaveIndex)
{
	if (!WaveDataTable) return;

	CurrentWaveIndex = WaveIndex;
	AliveEnemyCount = 0;
	ActiveSpawnTasks.Empty();

	// 데이터 테이블에서 WaveIndex에 해당하는 행(Row) 이름 생성 (예: "Wave_01")
	FName RowName = FName(*FString::Printf(TEXT("Wave_%02d"), CurrentWaveIndex));
	FString ContextString = TEXT("Wave Manager Context");
	
	// 데이터 테이블 정보의 포인터를 넘긴다(메모리에 유지)
	const FWaveInfo* WaveInfo = WaveDataTable->FindRow<FWaveInfo>(RowName, ContextString);

	if (WaveInfo)
	{
		SetWaveState(EWaveState::Preparing);

		// DelayBeforeStart 시간만큼 대기 후 BeginSpawning 호출
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UWaveManagerComponent::BeginSpawning,WaveInfo);
		GetWorld()->GetTimerManager().SetTimer(PrepareTimerHandle,TimerDel , WaveInfo->DelayBeforeStart, false);
	}
	else
	{
		// 더 이상 웨이브 데이터가 없으면 게임 클리어 처리 등
	}
}

void UWaveManagerComponent::OnMonsterDied(AActor* DeadMonster)
{
	AliveEnemyCount--;
	// 음수 방지
	if (AliveEnemyCount < 0) AliveEnemyCount = 0; 
    
	OnEnemyCountChanged.Broadcast(AliveEnemyCount);

	CheckWaveClearCondition();
}

void UWaveManagerComponent::BeginSpawning(const FWaveInfo* WaveInfo)
{
	SetWaveState(EWaveState::Spawning);

	// 각 몬스터 그룹별로 독립적인 타이머 세팅
	for (int32 i = 0; i < WaveInfo->SpawnGroups.Num(); ++i)
	{
		const FMonsterSpawnInfo& SpawnInfo = WaveInfo->SpawnGroups[i];

		FSpawnTask NewTask;
		NewTask.MonsterClass = SpawnInfo.MonsterClass;
		NewTask.TargetCount = SpawnInfo.SpawnCount;
		NewTask.SpawnedCount = 0;
        
		int32 TaskIndex = ActiveSpawnTasks.Add(NewTask);

		// 타이머에 TaskIndex를 매개변수(Payload)로 넘겨서 반복 실행
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UWaveManagerComponent::SpawnMonsterRoutine, TaskIndex);
		GetWorld()->GetTimerManager().SetTimer(ActiveSpawnTasks[TaskIndex].TimerHandle, TimerDel, SpawnInfo.SpawnInterval, true, 0.0f);
	}
}

void UWaveManagerComponent::SpawnMonsterRoutine(int32 TaskIndex)
{
	if (!ActiveSpawnTasks.IsValidIndex(TaskIndex)) return;

	FSpawnTask& Task = ActiveSpawnTasks[TaskIndex];

	// 1. 스포너 위치 찾기 (무작위)
	int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	AActor* SelectedSpawner = SpawnPoints[RandomIndex];
    
	FVector SpawnLocation = SelectedSpawner->GetActorLocation();
	FRotator SpawnRotation = SelectedSpawner->GetActorRotation();
	
	// 2. 오브젝트 풀에서 몬스터 가져오기 및 위치 세팅
	// 게임 스테이트에서 풀 컴포넌트를 가져와 몬스터 요청
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (UMonsterPoolComponent* PoolComp = GameMode->FindComponentByClass<UMonsterPoolComponent>())
		{
			// 스폰 및 활성화는 GetMonster 함수 내부에서 모두 처리됨
			ABase_Enemy* SpawnedMonster = PoolComp->GetMonster(Task.MonsterClass, SpawnLocation, SpawnRotation);
		}
	}
	

	// 카운트 증가
	Task.SpawnedCount++;
	AliveEnemyCount++;
	OnEnemyCountChanged.Broadcast(AliveEnemyCount); // UI에 몬스터 수 변경 알림

	// 해당 그룹의 목표 스폰 수를 채우면 타이머 정지
	if (Task.SpawnedCount >= Task.TargetCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(Task.TimerHandle);
		CheckWaveClearCondition();
	}
}

void UWaveManagerComponent::SetWaveState(EWaveState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		OnWaveStateChanged.Broadcast(CurrentState); // UI에 상태 변경 알림
	}
}

void UWaveManagerComponent::CheckWaveClearCondition()
{
	// 1. 모든 적이 죽었는가?
	if (AliveEnemyCount > 0) return;

	// 2. 모든 스폰 타이머가 종료되었는가? (아직 스폰 대기 중인 적이 있으면 안 됨)
	for (const FSpawnTask& Task : ActiveSpawnTasks)
	{
		if (Task.SpawnedCount < Task.TargetCount)
		{
			return; 
		}
	}

	// 위 두 조건을 모두 만족하면 웨이브 클리어
	SetWaveState(EWaveState::WaveCleared);
    
	// 다음 웨이브 대기, 혹은 보상 선택 로직 진행
}


