#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

/**
 * 싱글톤으로 태그를 관리하는 구조체
 * 태그 추가 시 태그 변수 선언 후 Initialize함수 내부에 태그 추가할 것.
 * 라이브코딩이 아닌 빌드를 해야 에디터에 적용된다.(애셋 매니저 초기화하면서 추가되기 때문)
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
	FGameplayTag Ability_Skill_Strike;
	FGameplayTag Ability_Skill_GunFire;
	
	FGameplayTag Event_Death;
	FGameplayTag Event_Equip;
	FGameplayTag Event_Hit;
	FGameplayTag Event_LevelUp;
	FGameplayTag Event_Montage_Fire;
	
	FGameplayTag GC_Combat_Damage;
	FGameplayTag GC_Combat_Damage_Shield;
	FGameplayTag GC_Combat_Damage_Health;
	FGameplayTag GC_Weapon_Fire;
	
	FGameplayTag Socket_Muzzle_L;
	FGameplayTag Socket_Muzzle_R;
	
	
protected:
	void AddAllTags(UGameplayTagsManager& Manager);
private:
	static FRsGameplayTags GameplayTags;
};
