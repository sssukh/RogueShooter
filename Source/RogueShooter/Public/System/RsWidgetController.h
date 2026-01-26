// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RsWidgetController.generated.h"

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
	void SetWidgetControllerParams(UAbilitySystemComponent* InASC);
	
	// 감시 함수 
	virtual void BindCallbacksToDependencies();
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	// 내부 콜백 
	void OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);
	
};
