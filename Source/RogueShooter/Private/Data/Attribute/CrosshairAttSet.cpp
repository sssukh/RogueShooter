// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Attribute/CrosshairAttSet.h"

#include "Net/UnrealNetwork.h"

UCrosshairAttSet::UCrosshairAttSet()
{
}

void UCrosshairAttSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UCrosshairAttSet,SpreadMin,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCrosshairAttSet,SpreadMax,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCrosshairAttSet,SpreadIncreasePerShot,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UCrosshairAttSet,SpreadRecoveryRate,COND_None,REPNOTIFY_Always)
	
}

void UCrosshairAttSet::OnRep_SpreadMin(const FGameplayAttributeData& OldSpreadMin)
{
}

void UCrosshairAttSet::OnRep_SpreadMax(const FGameplayAttributeData& OldSpreadMax)
{
}

void UCrosshairAttSet::OnRep_SpreadIncreasePerShot(const FGameplayAttributeData& OldSpreadIncreasePerShot)
{
}

void UCrosshairAttSet::OnRep_SpreadRecoveryRate(const FGameplayAttributeData& OldSpreadRecoveryRate)
{
}
