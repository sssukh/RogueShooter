// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RsBaseWidgetController.h"
#include "UnitWidgetController.generated.h"

// 1. 체력용 델리게이트 (숫자 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

// 2. 상태 이상용 델리게이트 (태그 전달 - 예: 기절 태그가 켜졌는지 꺼졌는지)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusEffectChangedSignature, const FGameplayTag&, Tag, bool, bIsActive);

class UHealthSet;
/**
 * 오브젝트에 필요한 UI를 관리하는 위젯 컨트롤러
 * 주로 캐릭터와 적의 체력 혹은 상태 이상 등의 UI를 관리한다.
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class ROGUESHOOTER_API UUnitWidgetController : public URsBaseWidgetController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnShieldChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnStatusEffectChangedSignature OnStatusEffectChanged; // 상태 이상 아이콘 제어용
public:
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params) override;;
	// virtual void SetWidgetControllerParams(FWidgetControllerParams Params) override;

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	
protected:
	// 태그 리스너 콜백 함수 (내부용)
	void OnStatusTagChanged(const FGameplayTag Tag, int32 NewCount);
protected:
	UPROPERTY(BlueprintReadOnly,Category = "Unit Widget Controller") 
	TObjectPtr<const UHealthSet> UnitHealthSet;
	
	// TODO : 상태이상 관리하는 태그 컨테이너 변수 추가
};
