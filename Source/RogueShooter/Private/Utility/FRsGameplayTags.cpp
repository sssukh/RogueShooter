#include "Utility/FRsGameplayTags.h"

#include "ChaosVDSQVisitorHelpers.h"

FRsGameplayTags FRsGameplayTags::GameplayTags;

void FRsGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FRsGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// 변수 = AddNativeGameplayTag("태그.이름", "설명")
	
	Data_Stat_Health = Manager.AddNativeGameplayTag(FName("Data.Stat.Health"),TEXT("체력"));
	Data_Stat_MaxHealth = Manager.AddNativeGameplayTag(FName("Data.Stat.MaxHealth"),TEXT("최대 체력"));
	Data_Stat_AttackPower = Manager.AddNativeGameplayTag(FName("Data.Stat.AttackPower"),TEXT("공격력"));
	Data_Stat_AttackSpeed = Manager.AddNativeGameplayTag(FName("Data.Stat.AttackSpeed"),TEXT("공격속도"));
	Data_Stat_Speed = Manager.AddNativeGameplayTag(FName("Data.Stat.Speed"),TEXT("이동속도"));
	Data_Stat_Critical = Manager.AddNativeGameplayTag(FName("Data.Stat.Critical"),TEXT("치명타확률"));
	Data_Stat_CriticalDamage = Manager.AddNativeGameplayTag(FName("Data.Stat.CriticalDamage"),TEXT("치명타 데미지"));
	Data_Stat_Defense = Manager.AddNativeGameplayTag(FName("Data.Stat.Defense"),TEXT("방어력"));
	
	Status_Dead = Manager.AddNativeGameplayTag(FName("Status.Dead"),TEXT("죽은 상태"));
	Status_Debuff = Manager.AddNativeGameplayTag(FName("Status.Debuff"),TEXT("디버프"));
	Status_Debuff_Stun = Manager.AddNativeGameplayTag(FName("Status.Debuff.Stun"),TEXT("디버프 스턴"));
	
	Event_Death = Manager.AddNativeGameplayTag(FName("Event.Death"),TEXT("죽음"));
	Event_Equip = Manager.AddNativeGameplayTag(FName("Event.Equip"),TEXT("아이템 착용"));
	Event_Hit = Manager.AddNativeGameplayTag(FName("Event.Hit"),TEXT("피격"));
	
	GC_Combat_Damage = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage"),TEXT("데미지"));
	GC_Combat_Damage_Health = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage.Health"),TEXT("체력에 데미지"));
	GC_Combat_Damage_Shield = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage.Shield"),TEXT("실드에 데미지"));
	
}
