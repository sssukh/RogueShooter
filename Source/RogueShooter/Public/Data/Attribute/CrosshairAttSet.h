// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CrosshairAttSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UCrosshairAttSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCrosshairAttSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats" ,ReplicatedUsing = OnRep_SpreadMin)
	FGameplayAttributeData SpreadMin;
	ATTRIBUTE_ACCESSORS(UCrosshairAttSet, SpreadMin);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats" ,ReplicatedUsing = OnRep_SpreadMax)
	FGameplayAttributeData SpreadMax;
	ATTRIBUTE_ACCESSORS(UCrosshairAttSet, SpreadMax);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats" ,ReplicatedUsing = OnRep_SpreadIncreasePerShot)
	FGameplayAttributeData SpreadIncreasePerShot;
	ATTRIBUTE_ACCESSORS(UCrosshairAttSet, SpreadIncreasePerShot);
	
	UPROPERTY(BlueprintReadOnly, Category = "Stats" ,ReplicatedUsing = OnRep_SpreadRecoveryRate)
	FGameplayAttributeData SpreadRecoveryRate;
	ATTRIBUTE_ACCESSORS(UCrosshairAttSet, SpreadRecoveryRate);
	
	UFUNCTION()
	virtual void OnRep_SpreadMin(const FGameplayAttributeData& OldSpreadMin);
	
	UFUNCTION()
	virtual void OnRep_SpreadMax(const FGameplayAttributeData& OldSpreadMax);
	
	UFUNCTION()
	virtual void OnRep_SpreadIncreasePerShot(const FGameplayAttributeData& OldSpreadIncreasePerShot);
	
	UFUNCTION()
	virtual void OnRep_SpreadRecoveryRate(const FGameplayAttributeData& OldSpreadRecoveryRate);
};
