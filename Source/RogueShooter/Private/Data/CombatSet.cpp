// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CombatSet.h"

#include "Net/UnrealNetwork.h"

UCombatSet::UCombatSet()
{
}

void UCombatSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,AttackPower,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,AttackSpeed,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,Critical,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,CriticalDamage,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,Speed,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,Defense,COND_None,REPNOTIFY_Always)
	

}

void UCombatSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,AttackPower,OldAttackPower);
	
}

void UCombatSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,AttackSpeed,OldAttackSpeed);
	
}

void UCombatSet::OnRep_Critical(const FGameplayAttributeData& OldCritical)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,Critical,OldCritical);
	
}

void UCombatSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage)
{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,CriticalDamage,OldCriticalDamage);

}

void UCombatSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,Speed,OldSpeed);
	
}

void UCombatSet::OnRep_Defense(const FGameplayAttributeData& OldDefense)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatSet,Defense,OldDefense);
	
}
