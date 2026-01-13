// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ExpSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelChanged,float,NewExpLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExpChanged,float,NewExp);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UExpSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UExpSet();
	
	ATTRIBUTE_ACCESSORS(ThisClass,MaxExpLevel);
	ATTRIBUTE_ACCESSORS(ThisClass,ExpLevel);
	ATTRIBUTE_ACCESSORS(ThisClass,MaxExpGained);
	ATTRIBUTE_ACCESSORS(ThisClass,ExpGained);
	ATTRIBUTE_ACCESSORS(ThisClass,DropExp);
	ATTRIBUTE_ACCESSORS(ThisClass,IncomingExp);
	
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintAssignable)
	mutable FLevelChanged OnLevelUp;
	
	UPROPERTY(BlueprintAssignable)
	mutable FExpChanged OnExpChange;

	
protected:
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData MaxExpLevel;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData ExpLevel;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData MaxExpGained;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData ExpGained;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData DropExp;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData IncomingExp;
};
