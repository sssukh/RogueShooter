#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

/**
 * 싱글톤으로 태그를 관리하는 구조체
 */
struct FRsGameplayTags
{
public:
	static const FRsGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();
	
	// 태그 변수 선언
	FGameplayTag Data_Stat_Health;
	FGameplayTag Data_Stat_MaxHealth;
	FGameplayTag Data_Stat_AttackPower;
	FGameplayTag Data_Stat_AttackSpeed;
	FGameplayTag Data_Stat_Speed;
	FGameplayTag Data_Stat_Critical;
	FGameplayTag Data_Stat_CriticalDamage;
	FGameplayTag Data_Stat_Defense;
	
	FGameplayTag Data_Duration;
	
	
	FGameplayTag Status_Dead;
	FGameplayTag Status_Debuff;
	FGameplayTag Status_Debuff_Stun;
	FGameplayTag Status_Cooldown_SKill1;
	FGameplayTag Status_Cooldown_SKill2;
	FGameplayTag Status_Cooldown_SKill3;
	FGameplayTag Status_Cooldown_SKill4;
	FGameplayTag Status_Cooldown_SKill5;
	
	
	FGameplayTag Ability_Skill;
	FGameplayTag Ability_Skill_Dash;
	
	
	FGameplayTag Event_Death;
	FGameplayTag Event_Equip;
	FGameplayTag Event_Hit;
	
	FGameplayTag GC_Combat_Damage;
	FGameplayTag GC_Combat_Damage_Shield;
	FGameplayTag GC_Combat_Damage_Health;
	
	
protected:
	void AddAllTags(UGameplayTagsManager& Manager);
private:
	static FRsGameplayTags GameplayTags;
};
