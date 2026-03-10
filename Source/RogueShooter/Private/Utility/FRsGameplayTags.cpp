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
	
	Data_Duration = Manager.AddNativeGameplayTag(FName("Data.Duration"),TEXT("기간"));
	
	Status_Dead = Manager.AddNativeGameplayTag(FName("Status.Dead"),TEXT("죽은 상태"));
	Status_Debuff = Manager.AddNativeGameplayTag(FName("Status.Debuff"),TEXT("디버프"));
	Status_Debuff_Stun = Manager.AddNativeGameplayTag(FName("Status.Debuff.Stun"),TEXT("디버프 스턴"));
	
	Status_Cooldown_SKill1 = Manager.AddNativeGameplayTag(FName("Status.Cooldown.Skill1"),TEXT("Skill1 쿨다운"));
	Status_Cooldown_SKill2 = Manager.AddNativeGameplayTag(FName("Status.Cooldown.Skill2"),TEXT("Skill2 쿨다운"));
	Status_Cooldown_SKill3 = Manager.AddNativeGameplayTag(FName("Status.Cooldown.Skill3"),TEXT("Skill3 쿨다운"));
	Status_Cooldown_SKill4 = Manager.AddNativeGameplayTag(FName("Status.Cooldown.Skill4"),TEXT("Skill4 쿨다운"));
	Status_Cooldown_SKill5 = Manager.AddNativeGameplayTag(FName("Status.Cooldown.Skill5"),TEXT("Skill5 쿨다운"));
	
	Ability_Skill = Manager.AddNativeGameplayTag(FName("Ability.Skill"),TEXT("스킬 태그"));
	Ability_Skill_Dash = Manager.AddNativeGameplayTag(FName("Ability.Skill.Dash"),TEXT("스킬 대쉬 태그"));
	Ability_Skill_Strike = Manager.AddNativeGameplayTag(FName("Ability.Skill.Strike"),TEXT("스킬 폭격 태그"));
	Ability_Skill_GunFire = Manager.AddNativeGameplayTag(FName("Ability.Skill.GunFire"),TEXT("스킬 총격"));
	Ability_Skill_Skill1 = Manager.AddNativeGameplayTag(FName("Ability.Skill.Skill1"),TEXT("1번 슬롯 스킬"));
	Ability_Skill_Skill1_DoubleTap = Manager.AddNativeGameplayTag(FName("Ability.Skill.Skill1.DoubleTap"),TEXT("더블 탭"));
	Ability_Skill_Skill2_PiercingRay = Manager.AddNativeGameplayTag(FName("Ability.Skill.Skill2.PiercingRay"),TEXT("관통의 빛"));
	Ability_Skill_Skill3_Slide = Manager.AddNativeGameplayTag(FName("Ability.Skill.Skill3.Slide"),TEXT("슬라이딩"));
	Ability_Skill_Skill4_BulletStorm = Manager.AddNativeGameplayTag(FName("Ability.Skill.Skill4.BulletStorm"),TEXT("탄막 전개"));
	
	
	Ability_Move = Manager.AddNativeGameplayTag(FName("Ability.Move"),TEXT("움직임"));
	Ability_Attack = Manager.AddNativeGameplayTag(FName("Ability.Attack"),TEXT("공격"));
	
	Event_Death = Manager.AddNativeGameplayTag(FName("Event.Death"),TEXT("죽음"));
	Event_Equip = Manager.AddNativeGameplayTag(FName("Event.Equip"),TEXT("아이템 착용"));
	Event_Hit = Manager.AddNativeGameplayTag(FName("Event.Hit"),TEXT("피격"));
	Event_LevelUp = Manager.AddNativeGameplayTag(FName("Event.LevelUp"),TEXT("레벨업"));
	Event_Montage_Fire = Manager.AddNativeGameplayTag(FName("Event.Montage.Fire"),TEXT("사격"));
	Event_Movement_HitReaction = Manager.AddNativeGameplayTag(FName("Event.Movement.HitReaction"),TEXT("피격 반응"));
	
	
	GC_Combat_Damage = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage"),TEXT("데미지"));
	GC_Combat_Damage_Health = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage.Health"),TEXT("체력에 데미지"));
	GC_Combat_Damage_Shield = Manager.AddNativeGameplayTag(FName("GameplayCue.Combat.Damage.Shield"),TEXT("실드에 데미지"));
	GC_Weapon_Fire = Manager.AddNativeGameplayTag(FName("GameplayCue.Weapon.Fire"),TEXT("발사"));
	GC_Weapon_Fire_DoubleTap = Manager.AddNativeGameplayTag(FName("GameplayCue.Weapon.Fire.DoubleTap"),TEXT("Drifter DoubleTap GC"));
}
