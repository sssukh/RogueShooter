// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask/AbilityTask_RepeatAction.h"

UAbilityTask_RepeatAction* UAbilityTask_RepeatAction::RepeatAction(UGameplayAbility* OwningAbility,
	float TimeBetweenActions, int32 TotalActionCount)
{
	// 태스크 인스턴스 생성
	UAbilityTask_RepeatAction* MyObj = NewAbilityTask<UAbilityTask_RepeatAction>(OwningAbility);
    
	// 매개변수 초기화
	MyObj->Interval = TimeBetweenActions;
	MyObj->TargetCount = TotalActionCount;
	MyObj->CurrentCount = 0;

	return MyObj;
}

void UAbilityTask_RepeatAction::Activate()
{
	Super::Activate();
	
	if (TargetCount <= 0)
	{
		OnFinished.Broadcast(-1);
		EndTask();
		return;
	}

	// Activate되자마자 첫 번째 발사를 즉시 실행 (선택 사항: 딜레이 후 쏘려면 바로 SetTimer 호출)
	PerformAction();
}

void UAbilityTask_RepeatAction::OnDestroy(bool bInOwnerFinished)
{
	// 어빌리티가 중간에 강제 취소되었을 때, 돌아가고 있던 타이머를 확실하게 꺼줌 (핵심 안전장치)
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_RepeatAction::PerformAction()
{
	// 어빌리티가 유효한지 안전 검사
	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	// 1. 발사 핀 실행 (블루프린트로 신호 보냄)
	OnPerformAction.Broadcast(CurrentCount);
	CurrentCount++;

	// 2. 목표 횟수에 도달했는지 체크
	if (CurrentCount >= TargetCount)
	{
		// 완료 핀 실행 후 태스크 종료
		OnFinished.Broadcast(-1);
		EndTask(); 
	}
	else
	{
		// 3. 아직 남았다면 다음 발사를 위해 타이머 세팅
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_RepeatAction::PerformAction, Interval, false);
	}
}
