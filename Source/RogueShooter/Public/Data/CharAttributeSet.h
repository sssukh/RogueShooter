// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharAttributeSet.generated.h"

//  Getter, Setter, Init 함수를 자동으로 만들어주는 매크로
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UCharAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCharAttributeSet();
	
	// 공격력
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, AttackPower);

	// 공격 속도
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, AttackSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Critical;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, Critical);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, CriticalDamage);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Hp;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, Hp);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats")
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UCharAttributeSet, Speed);
	
};
