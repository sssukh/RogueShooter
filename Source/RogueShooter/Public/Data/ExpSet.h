// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ExpSet.generated.h"


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
	ATTRIBUTE_ACCESSORS(ThisClass,IncomingExp);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	
public:
	UPROPERTY(BlueprintReadOnly,Category = "Attribute", ReplicatedUsing = OnRep_MaxExpLevel)
	FGameplayAttributeData MaxExpLevel;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute", ReplicatedUsing = OnRep_ExpLevel)
	FGameplayAttributeData ExpLevel;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute", ReplicatedUsing = OnRep_MaxExpGained)
	FGameplayAttributeData MaxExpGained;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute", ReplicatedUsing = OnRep_ExpGained)
	FGameplayAttributeData ExpGained;
	
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData IncomingExp;
	
public:
	UFUNCTION()
	virtual void OnRep_MaxExpLevel(const FGameplayAttributeData& OldMaxExpLevel);
	
	UFUNCTION()
	virtual void OnRep_ExpLevel(const FGameplayAttributeData& OldExpLevel);
	
	UFUNCTION()
	virtual void OnRep_MaxExpGained(const FGameplayAttributeData& OldMaxExpGained);
	
	UFUNCTION()
	virtual void OnRep_ExpGained(const FGameplayAttributeData& OldExpGained);
	
	
};
