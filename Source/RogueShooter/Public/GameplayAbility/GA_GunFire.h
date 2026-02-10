// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_Skill.h"
#include "Abilities/GameplayAbility.h"
#include "GA_GunFire.generated.h"

class ABase_Projectile;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UGA_GunFire : public UGA_Skill
{
	GENERATED_BODY()
public:
	UGA_GunFire();
	
	virtual void ExecuteSkillLogic_Implementation(float ChargeAmount) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="GAS | Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="GAS | Config")
	TSubclassOf<ABase_Projectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GAS")
	FVector SpawnTransformOffset;
};
