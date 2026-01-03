// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_EnemyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "UObject/FastReferenceCollector.h"
#include "Utility/FRsGameplayTags.h"

UGA_EnemyAttack::UGA_EnemyAttack()
{
}

void UGA_EnemyAttack::OnHitPlayer(AActor* TargetActor)
{
	// 필요한 정보 가져오기 
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo(); // 이 GA를 발동시킨 적 ASC
	UAbilitySystemComponent* TargetASC  = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if (!SourceASC || !TargetActor) return;
	
	// Spec 생성
	//  GE_EnemyDamage 클래스를 사용해서 Spec 생성
	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // 누가 때렸는지 기록
	
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,1.0f,ContextHandle);
	
	if (SpecHandle.IsValid())
	{
		// 데미지 수치 주입 (SetByCaller)
		// 적의 공격력이 50이라고 가정. 음수로 변환하여 전달
		float AttackDamage = -50.0f;
		
		// AttributeSet에서 직접 가져오려면 GetNumericAttribute 사용
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle,
			FRsGameplayTags::Get().Data_AttackPower,
			AttackDamage);
		
		// 플레이어에게 적용 
		SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
	}
}
