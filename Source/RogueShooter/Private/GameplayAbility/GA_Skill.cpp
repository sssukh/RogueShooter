// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_Skill.h"

#include "AbilitySystemComponent.h"
#include "Utility/FRsGameplayTags.h"

UGA_Skill::UGA_Skill()
{
	CooldownDurationTag = FRsGameplayTags::Get().Data_Duration;
}

void UGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 자동 커밋 옵션이 켜져 있을 때만 실행 
	if (bAutoCommit)
	{
		// 1. 커밋 (비용/쿨타임 지불 시도)
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
	}
	else
	{
		// cost 적용 로직 필요하면 구현
		// if (!ApplyCost(Handle,ActorInfo,ActivationInfo))
		// {
		// 	
		// }
	}
	// 2. 몽타주 자동 재생 (설정되어 있다면)
	if (SkillMontage)
	{
		// AbilityTask_PlayMontageAndWait를 C++에서 생성해서 실행하거나
		// 단순 재생 후 BP에게 타이밍을 맡길 수도 있습니다.
	}

	// 3.  블루프린트 로직 실행 
	BP_OnActivateSkill();
	
	// bAutoCommit이 꺼져있다면 EndAbility 앞에 ApplyCooldown 실행 
}

void UGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// [자동화 로직]
	// bAutoCommit이 꺼져 있다? = "이 스킬은 내가 수동으로 흐름을 제어하겠다(연사/홀드)"는 뜻.
	// 사용자가 별도로 ApplyCooldown을 안 불렀다면, 스킬이 끝나는 지금이 쿨타임 적용 타이밍이다!
	if (!bAutoCommit)
	{
		// 만약 취소 시 쿨타임을 안 주려면 if (!bWasCancelled) 조건을 추가
        
		ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

FVector UGA_Skill::GetMouseCursorLocation()
{
	return FVector();
}


