// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Attribute/HealthSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Utility/FRsGameplayTags.h"
#include "Utility/RSLog.h"

UHealthSet::UHealthSet()
{
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// 변경된 어트리뷰트가 health인지 확인
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float LocalIncomingDamage = GetIncomingDamage();
		
		// RS_LOG_SCREEN(TEXT("%s got %f Damage"),*GetOwningActor()->GetName(),LocalIncomingDamage)
		
		// meta attribute 초기화
		SetIncomingDamage(0.0f);
		
		if (GetShield()>0.0f)
		{
			float IncomingShieldDamage = LocalIncomingDamage>GetShield()?GetShield():LocalIncomingDamage;
			
			// 쉴드를 넘어서는 대미지 (0 이상)
			LocalIncomingDamage = FMath::Max(0.0f,LocalIncomingDamage-GetShield());
			
			float OldShield = GetShield();
			
			SetShield(OldShield-IncomingShieldDamage);
			
			// GameplayCue로 shield 피격시 이펙트 발동
			FGameplayCueParameters CueParams;
			CueParams.RawMagnitude = IncomingShieldDamage;
			CueParams.Location = Data.Target.GetAvatarActor()->GetActorLocation();
			CueParams.EffectCauser = Data.EffectSpec.GetEffectContext().GetEffectCauser();
			
			// 타겟에게 GC를 실행하라고 명령 
			Data.Target.AbilityActorInfo->AbilitySystemComponent->ExecuteGameplayCue(
				FRsGameplayTags::Get().GC_Combat_Damage_Shield,CueParams);
		}
		
		if (LocalIncomingDamage>0.0f)
		{
			const float FinalDamage = LocalIncomingDamage;
			
			const float NewHealth = GetHealth() - FinalDamage;
			
			SetHealth(NewHealth);
			
			
			// GameplayCue로 health 피격시 이펙트 발동
			FGameplayCueParameters CueParams;
			CueParams.RawMagnitude = FinalDamage;
			CueParams.Location = Data.Target.GetAvatarActor()->GetActorLocation();
			CueParams.EffectCauser = Data.EffectSpec.GetEffectContext().GetEffectCauser();
			
			// 타겟에게 GC를 실행하라고 명령 
			Data.Target.AbilityActorInfo->AbilitySystemComponent->ExecuteGameplayCue(
				FRsGameplayTags::Get().GC_Combat_Damage_Health,CueParams);
		}
		
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
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet,Shield,COND_None,REPNOTIFY_Always);
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
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Max(0.0f,NewValue);
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

void UHealthSet::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet,Shield,OldShield);
	
}
