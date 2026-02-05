// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "RogueShooter/RSEnumStruct.h"
#include "HealthSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// 데미지 받을 때 적용할 델리게이트들 추가하기.(플로팅 데미지 위젯 용 등)
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UHealthSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UHealthSet();
	
	// GE가 적용된 직후 호출되는 함수 
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	
	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthSet, Health);
	
	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UHealthSet, Shield);
	
	// 메타용 들어오는 데미지
	UPROPERTY(Blueprintreadonly,Category = "Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UHealthSet,IncomingDamage);
	
	// // Attribute 변화 시 체력바 UI에게 값을 갱신할 델리게이트
	// mutable FAttributeDataChanged OnMaxHealthChanged;
	// mutable FAttributeDataChanged OnCurrentHealthChanged;
	// mutable FAttributeDataChanged OnCurrentShieldDamaged;
	//
	// // 체력바에 영향을 주면 floating text나 체력바에 영향
	// mutable FShieldDamaged OnShieldDamaged;
	// mutable FHealthDamaged OnHealthDamaged;
	// mutable FHealthHealed OnHealthHealed;
public:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);
};
