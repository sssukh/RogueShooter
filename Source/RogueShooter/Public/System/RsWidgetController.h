// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsBaseWidgetController.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UObject/Object.h"
#include "RsWidgetController.generated.h"


struct FGameplayEventData;
class UExpSet;
class UCombatSet;
class UHealthSet;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangeSignature, FGameplayTag,CooldownTag,bool,bIsCooldown,float,Duration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillInfoLoadedSignature, const FGameplayTag&, AbilityTag, const FSkillInfo&, RowData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignatureInHud, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitConfirmedSignature);

UCLASS(Blueprintable,BlueprintType)
class ROGUESHOOTER_API URsWidgetController : public URsBaseWidgetController
{
	GENERATED_BODY()
public:
	URsWidgetController();
	
	// 쿨다운 시작 및 종료 시 알림
	UPROPERTY(BlueprintAssignable, Category = "GAS | Message")
	FOnCooldownChangeSignature OnCooldownChange;
	
	// 스킬 정보 알림
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnSkillInfoLoadedSignature OnSkillInfoLoaded;
	
	// EXP 관련 델리게이트 
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnExpChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnMaxExpChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnLevelChanged;
	
	// Health 관련 델리게이트
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAttributeChangedSignatureInHud OnShieldChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnHitConfirmedSignature OnHitConfirmedSignature;
	
	// 초기화 함수
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& WcParams) override;
	
	// 감시 함수 
	// 감시할 ASC의 AttributeSet이 변하면 broadcast하는 람다를 등록.
	virtual void BindCallbacksToDependencies() override;
	
	// BindCallbacksToDependencies로 등록한 오브젝트들에게 초기값 방송
	virtual void BroadcastInitialValues() override;
	
	void SetTagsToListen(FGameplayTagContainer Tags);
	
	bool GetCooldownInfo(const FGameplayTag& CooldownTag, float& OutRemaining, float& OutDuration);
	
	FORCEINLINE TArray<FGameplayTag> GetTagsToListen() const {return TagsToListen;}
	
	void BroadcastInitialAbilityInfo();
	
	void OnHitEventReceived(const FGameplayEventData* Payload);
protected:
	void BindExpBarCallbacks();
	
	void BindHealthBarCallbacks();
protected:
	
	UPROPERTY(BlueprintReadOnly,Category = "Widget Controller") 
	TObjectPtr<const UExpSet> CharExpSet;
	
	UPROPERTY(BlueprintReadOnly,Category = "Widget Controller") 
	TObjectPtr<const UHealthSet> CharHealthSet;
	
	// 내부 콜백 
	void OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data | Config")
	TArray<FGameplayTag> TagsToListen;
	
	// 스킬 데이터 테이블 참조
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data | Config")
	TObjectPtr<UDataTable> SkillInfoDataTable;
	
	// Hit 수신할 태그 
	UPROPERTY(EditDefaultsOnly, Category="GAS|Events")
	FGameplayTag HitEventTag;
};
