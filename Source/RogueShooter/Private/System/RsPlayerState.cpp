// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Data/Attribute/CombatSet.h"
#include "Data/Attribute/ExpSet.h"
#include "Data/Attribute/HealthSet.h"
#include "Data/Attribute/CrosshairAttSet.h"

ARsPlayerState::ARsPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AbilitySystemComponent->SetIsReplicated(true);
	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	HealthAttributes = CreateDefaultSubobject<UHealthSet>(TEXT("HeathAttributes"));
	CombatAttributes = CreateDefaultSubobject<UCombatSet>(TEXT("CombatAttributes"));
	ExpAttributes = CreateDefaultSubobject<UExpSet>(TEXT("ExpAttributes"));
	CrosshairAttributes = CreateDefaultSubobject<UCrosshairAttSet>(TEXT("CrosshairAttributes"));
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
