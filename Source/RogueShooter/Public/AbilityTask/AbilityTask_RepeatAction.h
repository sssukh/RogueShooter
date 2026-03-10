// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_RepeatAction.generated.h"

// 블루프린트 노드의 실행 핀이 될 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRepeatActionDelegate, int32, ActionNumber);

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UAbilityTask_RepeatAction : public UAbilityTask
{
	GENERATED_BODY()
public:
	// 발사할 때마다 실행될 핀
	UPROPERTY(BlueprintAssignable)
	FOnRepeatActionDelegate OnPerformAction;

	// 모든 발사가 끝났을 때 실행될 핀
	UPROPERTY(BlueprintAssignable)
	FOnRepeatActionDelegate OnFinished;

	// 블루프린트에서 노드를 생성할 때 사용할 Static 함수
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_RepeatAction* RepeatAction(UGameplayAbility* OwningAbility, float TimeBetweenActions, int32 TotalActionCount);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override; // 어빌리티 취소 시 안전장치

private:
	void PerformAction(); // 타이머가 호출할 내부 함수

	int32 CurrentCount;
	int32 TargetCount;
	float Interval;
	FTimerHandle TimerHandle;
};
