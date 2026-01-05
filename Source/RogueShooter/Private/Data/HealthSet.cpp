// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/HealthSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Utility/FRsGameplayTags.h"

UHealthSet::UHealthSet()
	:MaxHealth(100.0f)
{
	InitHealth(GetMaxHealth());
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// 변경된 어트리뷰트가 health인지 확인
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 체력이 0보다 작아지지 않게 clamp
		SetHealth(FMath::Clamp(GetHealth(),0.0f,GetMaxHealth()));
		
		// 사망 체크
		if (GetHealth() <=0.0f)
		{
			// 캐릭터에게 죽음 알림
			// Data.Target은 ASC이므로, AvatarActor(캐릭터)를 가져옴
			if (AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get())
			{
				// 이벤트를 쏘기 위한 Payload(데이터 보따리) 준비
				FGameplayEventData Payload;
				Payload.EventTag = FRsGameplayTags::Get().Event_Death;
				Payload.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator(); // 누가 죽였는지 정보 
				Payload.Target = TargetActor;
				
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, Payload.EventTag,Payload);
			}
		}
	}
}

void UHealthSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet,MaxHealth,COND_None,REPNOTIFY_Always);
}

void UHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 최대 체력은 1이상으로 설정
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(1.0f,NewValue);
	}
	// 현재 체력은 0이상 최대체력 이하
	else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.0f,GetMaxHealth());
	}
}

void UHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		OnMaxHealthChanged.Broadcast(OldValue,NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		OnCurrentHealthChanged.Broadcast(OldValue,NewValue);
	}
}

void UHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet,Health,OldHealth);
}

void UHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet,MaxHealth,OldMaxHealth);
}
