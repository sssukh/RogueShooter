// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ExpSet.h"

#include "GameplayEffectExtension.h"
#include "Character/Base_Character.h"
#include "Net/UnrealNetwork.h"
#include "Utility/RSLog.h"

UExpSet::UExpSet()
	: MaxExpLevel(1000.0f)
	, MaxExpGained(50.0f)
{
	InitExpLevel(1.0f);
	InitExpGained(0.0f);
	InitDropExp(0.0f);
}

void UExpSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,MaxExpGained,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,ExpGained,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,ExpLevel,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,MaxExpLevel,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,DropExp,COND_None,REPNOTIFY_Always)
}

void UExpSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetExpLevelAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpLevel());
		NewValue = FMath::TruncToFloat(NewValue);
	}
	else if (Attribute == GetMaxExpGainedAttribute())
	{
		RS_LOG_SCREEN(TEXT("Max Xp has Changed"))
	}
	// else if (Attribute == GetExpGainedAttribute())
	// {
	// 	NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpGained());
	// }
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
		
		RS_LOG_SCREEN(TEXT("Incoming : %f, Current : %f, Max : %f, Level : %f"),LocalIncomingXp,CurrentXp,NextMaxXp,CurrentLevel)
		
		if (LocalIncomingXp + CurrentXp >= NextMaxXp)
		{
			const float remainXp = CurrentXp + LocalIncomingXp - NextMaxXp;
			
			SetExpLevel(CurrentLevel + 1.0f);
			
			SetExpGained(remainXp);
			
			// SetMaxExpGained(NextMaxXp);
			
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
	}
}

void UExpSet::OnRep_MaxExpLevel(const FGameplayAttributeData& OldMaxExpLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExpSet,MaxExpLevel,OldMaxExpLevel);
	
}

void UExpSet::OnRep_ExpLevel(const FGameplayAttributeData& OldExpLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExpSet,ExpLevel,OldExpLevel);
	
}

void UExpSet::OnRep_MaxExpGained(const FGameplayAttributeData& OldMaxExpGained)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExpSet,MaxExpGained,OldMaxExpGained);
	
}

void UExpSet::OnRep_ExpGained(const FGameplayAttributeData& OldExpGained)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExpSet,ExpGained,OldExpGained);
	
}

void UExpSet::OnRep_DropExp(const FGameplayAttributeData& OldDropExp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UExpSet,DropExp,OldDropExp);
	
}
