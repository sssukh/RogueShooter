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
	
	Data_Health = Manager.AddNativeGameplayTag(FName("Data.Health"),TEXT("체력"));
	Data_MaxHealth = Manager.AddNativeGameplayTag(FName("Data.MaxHealth"),TEXT("최대 체력"));
	Data_AttackPower = Manager.AddNativeGameplayTag(FName("Data.AttackPower"),TEXT("공격력"));
	Data_AttackSpeed = Manager.AddNativeGameplayTag(FName("Data.AttackSpeed"),TEXT("공격속도"));
	Data_Speed = Manager.AddNativeGameplayTag(FName("Data.Speed"),TEXT("이동속도"));
	Data_Critical = Manager.AddNativeGameplayTag(FName("Data.Critical"),TEXT("치명타확률"));
	Data_CriticalDamage = Manager.AddNativeGameplayTag(FName("Data.CriticalDamage"),TEXT("치명타 데미지"));
	
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
