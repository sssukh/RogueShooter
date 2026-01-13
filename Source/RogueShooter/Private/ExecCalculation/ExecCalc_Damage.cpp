// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalculation/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "IMovieSceneModule.h"
#include "Data/HealthSet.h"
#include "Data/CombatSet.h"


struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);
	
	FDamageStatics()
	{
		// Snapshot : false(때리는 순간의 스탯을 가져온다), true(GE 생성 시점에 가져온다)
		// source : 공격자, target : 맞는 사람
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatSet,AttackPower,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatSet,Critical,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatSet,CriticalDamage,Source,false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatSet,Defense,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHealthSet,IncomingDamage,Target,false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().IncomingDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 스탯값 가져오기
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// 값 추출 
	float AttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef,EvaluationParameters , AttackPower);
	
	float Critical = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDef,EvaluationParameters , Critical);
	
	float CriticalDamage = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageDef,EvaluationParameters , CriticalDamage);
	
	// Target이 defense attribute를 소유하고있는지 확인
	float Defense = 0.f;
	if (TargetASC->HasAttributeSetForAttribute(UCombatSet::GetDefenseAttribute()))
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, Defense);
	
	// 대미지 공식 적용
	// 1. 치명타 시 치명타 대미지 적용.
	// 2. 방어력 적용 
	
	float Damage = FMath::RandRange(0.0f,1.0f)<Critical?AttackPower*CriticalDamage:AttackPower;
	
	Damage = FMath::Max<float>(AttackPower - Defense,0.0f);
	
	// 결과인 Damage를 meta Attribute인 IncomingDamage에 더하기 
	if (Damage>0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingDamageProperty,EGameplayModOp::AddBase,Damage));
	}
}
