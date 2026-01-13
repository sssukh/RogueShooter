// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CombatSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UCombatSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCombatSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	// 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UCombatSet, AttackPower);

	// 공격 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCombatSet, AttackSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Critical;
	ATTRIBUTE_ACCESSORS(UCombatSet, Critical);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UCombatSet, CriticalDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UCombatSet, Speed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UCombatSet, Defense);
};
