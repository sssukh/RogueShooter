// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_Skill.h"

#include "AbilitySystemComponent.h"
#include "Utility/FRsGameplayTags.h"

UGA_Skill::UGA_Skill()
{
}

void UGA_Skill::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// 설정한 GE 없으면 기본 로직 실행 
	if (!CooldownGameplayEffectClass)
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
		return;
	}
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;
	
	// Spec 생성
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownGameplayEffectClass,GetAbilityLevel(),MakeEffectContext(Handle,ActorInfo));
	if (!SpecHandle.IsValid()) return;
	
	// Duration 설정 (SetByCaller)
	SpecHandle.Data->SetSetByCallerMagnitude(CooldownDurationTag,CooldownDuration);
	
	// 쿨타임 태그 동적 추가
	SpecHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
	
	// 적용
	ApplyGameplayEffectSpecToOwner(Handle,ActorInfo,ActivationInfo,SpecHandle);
}

const FGameplayTagContainer* UGA_Skill::GetCooldownTags() const
{
	if (CooldownTags.IsEmpty())
		return Super::GetCooldownTags(); 
	
	return &CooldownTags;
}
