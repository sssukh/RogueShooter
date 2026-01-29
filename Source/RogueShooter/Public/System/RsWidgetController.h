// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsWidgetController.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	// 생성자: 4개의 핵심 데이터를 받아서 초기화
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

class UAbilitySystemComponent;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangeSignature, FGameplayTag,CooldownTag,bool,bIsCooldown,float,Duration);

UCLASS()
class ROGUESHOOTER_API URsWidgetController : public UObject
{
	GENERATED_BODY()
public:
	URsWidgetController();
	
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Message")
	FOnCooldownChangeSignature OnCooldownChange;
	
	// 초기화 함수 
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WcParams, FGameplayTagContainer InTagsToListen);
	
	// 감시 함수 
	virtual void BindCallbacksToDependencies();
	
	void SetTagsToListen(FGameplayTagContainer Tags);
	
	bool GetCooldownInfo(const FGameplayTag& CooldownTag, float& OutRemaining, float& OutDuration);
	
	FORCEINLINE const TArray<FGameplayTag> GetTagsToListen() const {return TagsToListen;}
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY()
	TObjectPtr<APlayerState> PS;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	// 내부 콜백 
	void OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);
	
	UPROPERTY()
	TArray<FGameplayTag> TagsToListen;
};
