// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Skill.generated.h"

/**
 * BaseSkill 클래스 
 * 
 * 모든 스킬에서 사용할 쿨타임 시스템을 적용함
 * 
 * 새로운 스킬을 추가할 때 마다 RSGameplayTag 클래스 내부에 스킬 태그를 추가할 것.
 */
UCLASS()
class ROGUESHOOTER_API UGA_Skill : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Skill();
	
	
	/**
	 * 블루프린트 에디터에서 설정한 Cooldown GE Class를 CommitAbility로 간단히 실행 가능하도록 오버라이드
	 * SetByCaller를 통해 동적으로 쿨타임을 제어한다.
	 * 
	 * @param Handle 
	 * @param ActorInfo 
	 * @param ActivationInfo 
	 */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	// 쿨다운 여부를 
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
public:
	// 데이터 애셋에 접근할 키 값 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	FGameplayTag AbilityTag;
	
	// 쿨타임 설정 값
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	float CooldownDuration;
	
	// 쿨타임에 관여할 태그들을 담은 태그 컨테이너 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTagContainer CooldownTags;
	
	// 외부에서 쿨타임을 설정할 때 필요한 태그
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTag CooldownDurationTag;
};
