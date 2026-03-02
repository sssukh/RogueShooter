// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Attribute/ExpSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Character/Base_Character.h"
#include "Net/UnrealNetwork.h"
#include "Utility/FRsGameplayTags.h"
#include "Utility/RSLog.h"

UExpSet::UExpSet()
{
}

void UExpSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,MaxExpGained,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,ExpGained,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,ExpLevel,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UExpSet,MaxExpLevel,COND_None,REPNOTIFY_Always)
}

void UExpSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetExpLevelAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpLevel());
		// NewValue = FMath::TruncToFloat(NewValue);
	}
	else if (Attribute == GetMaxExpGainedAttribute())
	{
		// RS_LOG_SCREEN(TEXT("Max Xp has Changed"))
	}
	else if (Attribute == GetExpGainedAttribute())
	{
		// NewValue = FMath::Clamp<float>(NewValue,0.0f,GetMaxExpGained());
		// RS_LOG_SCREEN(TEXT("Xp has Changed"))
	}
}

void UExpSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetIncomingExpAttribute())
	{
		
		float CurrentXp = GetExpGained();
		
		float CurrentLevel = GetExpLevel();
        
		float LocalIncomingXp = GetIncomingExp();
		
		SetIncomingExp(0.0f);
		
		// 커브에서 값 평가 (Evaluate)
		float NextMaxXp = 100.0f; // 기본값
		ABase_Character* Char = Cast<ABase_Character>(Data.Target.GetAvatarActor());
		if (Char)
		{
			// ContextString은 디버깅용 빈 문자열
			// 캐릭터에 있는 커브 테이블에서 값 가져오기 
			NextMaxXp = Char->GetMaxXpForLevel(CurrentLevel);
		}
		else
		{
			RS_LOG_ERROR(TEXT("Character의 클래스가 Base_Character가 아닙니다."))
			return;
		}
		
		// 갑자기 큰 경험치가 들어와서 레벨업이 여러번 일어남을 방지 
		float RemainXp = CurrentXp + LocalIncomingXp;
		while (RemainXp >= NextMaxXp)
		{
			RemainXp -= NextMaxXp;
			
			CurrentLevel+=1.0f;
			
			SetExpLevel(CurrentLevel);
			
			SetExpGained(RemainXp);
			
			FGameplayEventData Payload;
			Payload.EventTag = FRsGameplayTags::Get().Event_LevelUp;
			Payload.EventMagnitude = CurrentLevel; // 필요하면 현재 레벨을 담음
        
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), Payload.EventTag, Payload);
			
			NextMaxXp = Char->GetMaxXpForLevel(GetExpLevel());
		}
		
		SetExpGained(RemainXp);
			
		
		// RS_LOG_SCREEN(TEXT("ExpGained : %f, ExpMax : %f "),GetExpGained(),GetMaxExpGained())
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


