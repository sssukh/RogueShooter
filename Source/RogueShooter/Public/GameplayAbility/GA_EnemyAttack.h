// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_EnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UGA_EnemyAttack : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_EnemyAttack();
	
	UFUNCTION(BlueprintCallable)
	void OnHitPlayer(AActor* TargetActor);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GAS | Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};
