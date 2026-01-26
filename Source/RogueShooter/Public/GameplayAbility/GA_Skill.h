// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Skill.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UGA_Skill : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Skill();
	
	// GE Cooldown Class에 설정된 GE로 쿨타임 적용
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	float CooldownDuration;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTagContainer CooldownTags;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTag CooldownDurationTag;
};
