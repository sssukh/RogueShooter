// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ExpSet.h"

#include "GameplayEffectExtension.h"
#include "Character/Base_Character.h"
#include "Utility/RSLog.h"

UExpSet::UExpSet()
	: MaxExpLevel(1000.0f)
	, MaxExpGained(100.0f)
{
	InitExpLevel(1.0f);
	InitExpGained(0.0f);
	InitDropExp(0.0f);
}

void UExpSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetExpLevelAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpLevel());
		NewValue = FMath::TruncToFloat(NewValue);
	}
	else if (Attribute == GetExpGainedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpGained());
	}
}

void UExpSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		float CurrentXp = GetExpGained();
		
		const float CurrentLevel = GetExpLevel();
        
		float LocalIncomingXp = GetIncomingExp();
		
		SetIncomingExp(0.0f);
		
		// 커브에서 값 평가 (Evaluate)
		float NextMaxXp = 100.0f; // 기본값
		if (ABase_Character* Char = Cast<ABase_Character>(GetOwningActor()))
		{
			// ContextString은 디버깅용 빈 문자열
			// 캐릭터에 있는 커브 테이블에서 값 가져오기 
			NextMaxXp = Char->GetMaxXpForLevel(CurrentLevel);
		}
		
		if (LocalIncomingXp + CurrentXp >= NextMaxXp)
		{
			const float remainXp = CurrentXp + LocalIncomingXp - NextMaxXp;
			
			SetExpLevel(GetExpLevel() + 1.0f);
			
			SetExpGained(remainXp);
			
			if (OnLevelUp.IsBound())
			{
				OnLevelUp.Broadcast(GetExpLevel());
			}
		}
		else
		{
			SetExpGained(LocalIncomingXp + CurrentXp);
			
		}
		RS_LOG_SCREEN(TEXT("ExpGained : %f, ExpMax : %f "),GetExpGained(),GetMaxExpGained())
		
		if (OnExpChange.IsBound())
		{
			OnExpChange.Broadcast(GetExpGained());
		}
		
		// float OldExpLevel = GetExpLevel();
		// float NewExpLevel = FMath::TruncToFloat(GetExpGained()/10.0f) + 1.0f;
		// if (NewExpLevel>OldExpLevel)
		// {
		// 	SetExpLevel(NewExpLevel);
		// 	OnLevelUp.Broadcast(NewExpLevel);
		// }
	}
}
