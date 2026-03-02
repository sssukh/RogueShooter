// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RsPlayerState.generated.h"

class UCrosshairAttSet;
class UExpSet;
class UCombatSet;
class UHealthSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ARsPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ARsPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	FORCEINLINE UHealthSet* GetHealthAttributes() {return HealthAttributes;}

	FORCEINLINE UCombatSet* GetCombatAttributes() {return CombatAttributes;}
		
	FORCEINLINE UExpSet* GetExpAttributes() {return ExpAttributes;}
		
	FORCEINLINE UCrosshairAttSet* GetCrosshairAttributes() {return CrosshairAttributes;}
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UHealthSet> HealthAttributes;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "GAS")
	TObjectPtr<UCombatSet> CombatAttributes;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "GAS")
	TObjectPtr<UExpSet> ExpAttributes;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "GAS")
	TObjectPtr<UCrosshairAttSet> CrosshairAttributes;
};
