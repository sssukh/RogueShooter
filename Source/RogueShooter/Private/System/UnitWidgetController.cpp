// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UnitWidgetController.h"
#include "GameplayEffectTypes.h"
#include "Data/HealthSet.h"
#include "Utility/RSLog.h"


void UUnitWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	AbilitySystemComponent = Params.AbilitySystemComponent;
	
	if (AbilitySystemComponent)
	{
		UnitHealthSet = Cast<UHealthSet>(AbilitySystemComponent->GetAttributeSet(UHealthSet::StaticClass()));
	}
}

void UUnitWidgetController::BroadcastInitialValues()
{
	if (UnitHealthSet)
	{
		OnHealthChanged.Broadcast(UnitHealthSet->GetHealth());
		OnMaxHealthChanged.Broadcast(UnitHealthSet->GetMaxHealth());
	}
}

void UUnitWidgetController::BindCallbacksToDependencies()
{
	// Attribute Delegate 체력관리(숫자)
	if (UnitHealthSet)
	{
		FDelegateHandle Handle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UnitHealthSet->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				});
		if (Handle.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[Binding] Success! Handle is Valid."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[Binding] Failed! Handle is Invalid."));
		}
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UnitHealthSet->GetMaxHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnMaxHealthChanged.Broadcast(Data.NewValue);
				});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UnitHealthSet->GetShieldAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnShieldChanged.Broadcast(Data.NewValue);
				});
	}
	if (AbilitySystemComponent)
	{
		// 감시하고 싶은 태그들 (예: 기절, 화상)
		// 실제로는 FGameplayTag::RequestGameplayTag("Debuff.Stun") 등을 사용하거나 배열로 관리
		TArray<FGameplayTag> StatusTags; 
		// StatusTags.Add(FGameplayTag::RequestGameplayTag(FName("Debuff.Stun")));
		// StatusTags.Add(FGameplayTag::RequestGameplayTag(FName("Debuff.Burn")));

		for (const FGameplayTag& Tag : StatusTags)
		{
			// ASC에게 "이 태그 변하면 나한테 알려줘" 등록
			AbilitySystemComponent->RegisterGameplayTagEvent(
				Tag, 
				EGameplayTagEventType::NewOrRemoved // 생기거나 사라질 때만
			).AddUObject(this, &UUnitWidgetController::OnStatusTagChanged);
		}
	}
	
}

void UUnitWidgetController::OnStatusTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	// Count > 0 이면 걸린 것(True), 0이면 풀린 것(False)
	bool bIsActive = NewCount > 0;
    
	// UI에게 방송: "야, 기절(Stun) 태그 상태가 (켜짐/꺼짐)으로 변했어!"
	OnStatusEffectChanged.Broadcast(Tag, bIsActive);
}
