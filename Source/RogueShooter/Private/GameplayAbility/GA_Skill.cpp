// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_Skill.h"

#include "AbilitySystemComponent.h"
#include "VectorVM.h"
#include "Utility/FRsGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"

UGA_Skill::UGA_Skill()
{
	CooldownDurationTag = FRsGameplayTags::Get().Data_Duration;
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
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
	// // 2. 몽타주 자동 재생 (설정되어 있다면)
	// if (SkillMontage)
	// {
	// 	// AbilityTask_PlayMontageAndWait를 C++에서 생성해서 실행하거나
	// 	// 단순 재생 후 BP에게 타이밍을 맡길 수도 있습니다.
	// }
	//
	// // 3.  블루프린트 로직 실행 
	// BP_OnActivateSkill();
	
	// bAutoCommit이 꺼져있다면 EndAbility 앞에 ApplyCooldown 실행 
	
	switch (InputStyle)
	{
	case ESkillInputStyle::Instant:
	case ESkillInputStyle::Continuous:
		{
			ExecuteSkillLogic(1.0f); // 그냥 발사 (파워 100%)
			if (bAutoEndAbility)
				EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
		break;

	

	case ESkillInputStyle::Charging:
		{
			// 1. 차징 시작 시간 기록
			ChargeStartTime = GetWorld()->GetTimeSeconds();

			// 2. (옵션) 차징 시작 이펙트/애니메이션 재생 (여기서 PlayMontage 등 호출 가능)
			// TODO : 차징 이펙트 추가하기 
            
			// 3. 떼는 것을 기다림 (WaitInputRelease)
			UAbilityTask_WaitInputRelease* ReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
			ReleaseTask->OnRelease.AddDynamic(this, &UGA_Skill::OnReleaseInput); // 뗄 때 발사!
			ReleaseTask->ReadyForActivation();
		}
		break;

		// 루시안 궁 
	case ESkillInputStyle::Burst:
		{
			BurstLogic();
		}
		break;
	}
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
	if (InputStyle == ESkillInputStyle::Continuous)
		SpecHandle.Data->SetSetByCallerMagnitude(CooldownDurationTag,FireRate);
	else
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

void UGA_Skill::ExecuteSkillLogic_Implementation(float ChargeAmount)
{
}



void UGA_Skill::OnReleaseInput(float TimeHeld)
{
	// A. 차징 모드일 때
	if (InputStyle == ESkillInputStyle::Charging)
	{
		// 얼마나 모았는지 계산
		float ChargeDuration = GetWorld()->GetTimeSeconds() - ChargeStartTime;

		// 최소 시간도 안 채웠으면 취소 (혹은 약하게 발사)
		if (ChargeDuration < MinChargeTime)
		{
			CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
			return;
		}

		// 0.0 ~ 1.0 사이로 정규화 (Clamp)
		float ChargeAlpha = FMath::Clamp((ChargeDuration - MinChargeTime) / (MaxChargeTime - MinChargeTime), 0.0f, 1.0f);

		// 자식에게 전달! (이제 자식은 이 Alpha값으로 대미지나 투사체 크기를 조절)
		ExecuteSkillLogic(ChargeAlpha);

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	
}

void UGA_Skill::LoopLogic()
{
	// 매 발사마다 코스트 지불 시도
	if (CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		ExecuteSkillLogic(1.0f);
	}
	else
	{
		// 마나 없으면 사격 중지 (선택 사항)
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UGA_Skill::BurstLogic()
{
	// 카운트 초기화 
	CurrentBurstCount = 0;

	// 1. 엔진 내장 반복 태스크 생성
	UAbilityTask_Repeat* RepeatTask = UAbilityTask_Repeat::RepeatAction(this, BurstInterval, MaxBurstCount);

	if (RepeatTask)
	{
		// 2. 태스크의 실행 핀을 C++ 콜백 함수에 연결 (AddDynamic)
		RepeatTask->OnPerformAction.AddDynamic(this, &UGA_Skill::HandlePerformAction);
		RepeatTask->OnFinished.AddDynamic(this, &UGA_Skill::HandleRepeatFinished);

		// 3. 태스크 실행 시작
		RepeatTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UGA_Skill::HandlePerformAction(int32 ActionNumber)
{
	ExecuteSkillLogic(1.0f);
	++CurrentBurstCount;
}

void UGA_Skill::HandleRepeatFinished(int32 ActionNumber)
{
	// C++에서 깔끔하게 어빌리티를 종료합니다.
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}




