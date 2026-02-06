// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Data/CombatSet.h"
#include "Data/ExpSet.h"
#include "Data/HealthSet.h"

ARsPlayerState::ARsPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AbilitySystemComponent->SetIsReplicated(true);
	
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	HealthAttributes = CreateDefaultSubobject<UHealthSet>(TEXT("HeathAttributes"));
	CombatAttributes = CreateDefaultSubobject<UCombatSet>(TEXT("CombatAttributes"));
	ExpAttributes = CreateDefaultSubobject<UExpSet>(TEXT("ExpAttributes"));
}

UAbilitySystemComponent* ARsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
