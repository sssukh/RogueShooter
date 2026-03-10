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
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	void ApplyDamage(AActor* TargetActor);
	
	void ApplyGameplayEvent(AActor* TargetActor);
	
	void TriggerFireGameplayCue(FVector MuzzleLoc, FVector TargetLoc, const FHitResult& HitResult);
	
	virtual void ExecuteSkillLogic_Implementation(float ChargeAmount) override;
	


	// 1. 클라이언트 -> 서버 데이터 전송 헬퍼
	void SendTargetDataToServer(const FHitResult& HitResult);

	// 2. 서버 전용 수신 콜백 (여기서 데미지 적용)
	UFUNCTION()
	void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);
	
	UFUNCTION(BlueprintCallable,Category = "Weapon")
	void FireHitScan(FName SocketName);
	
	UFUNCTION(BlueprintCallable,Category = "Weapon")
	void GetTargetPoint(FName SocketName, FVector& TargetStart, FVector& TargetPoint, bool IsSpread = false );
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="GAS | Config")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="GAS | Config")
	TSubclassOf<ABase_Projectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GAS")
	FVector SpawnTransformOffset;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MuzzleIndex = 0;
	
};
