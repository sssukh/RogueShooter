// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/CombatSet.h"

#include "Net/UnrealNetwork.h"

UCombatSet::UCombatSet()
{
}

void UCombatSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,AttackPower,COND_None,REPNOTIFY_Always)
	// DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,AttackSpeed,COND_None,REPNOTIFY_Always)
	// DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,Critical,COND_None,REPNOTIFY_Always)
	// DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,CriticalDamage,COND_None,REPNOTIFY_Always)
	// DOREPLIFETIME_CONDITION_NOTIFY(UCombatSet,Speed,COND_None,REPNOTIFY_Always)

}
