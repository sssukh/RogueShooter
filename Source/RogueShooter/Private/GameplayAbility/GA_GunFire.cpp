// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_GunFire.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Base_Projectile.h"


UGA_GunFire::UGA_GunFire()
{
}

void UGA_GunFire::ExecuteSkillLogic_Implementation(float ChargeAmount)
{
	// 1. 대미지 GE 클래스 가져오기 (블루프린트에서 설정)
	if (!DamageEffectClass || !ProjectileClass)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextContext = SourceASC->MakeEffectContext();
	
	ContextContext.SetAbility(this);
	ContextContext.AddSourceObject(this);
	// ContextHandle.AddInstigator(Avatar,Weapon); // 무기정보 등을 넣기도 한다.
	
	// 2. 명세서(Spec) 생성
	// MakeOutgoingSpec을 하면 내(Source) 스탯을 기반으로 Context가 생성됩니다.
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextContext);

	// 3. (옵션) 동적으로 값 변경하기
	// 예: 차지샷이라서 대미지를 2배로 늘려야 한다면?
	// SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.DamageMultiplier")), 2.0f);

	// 4. 투사체 스폰을 위한 Transform 계산
	FTransform SpawnTransform = GetAvatarActorFromActorInfo()->GetActorTransform(); 
	// 실제로는 총구 소켓(Muzzle Socket) 위치를 가져와야 함
	// 임시
	FVector Forward = SpawnTransform.GetRotation().GetForwardVector();
	SpawnTransform.SetLocation(SpawnTransform.GetLocation() + SpawnTransformOffset);
	SpawnTransform.SetRotation(Forward.ToOrientationQuat());
	// 5. 투사체 스폰 (Deferred Spawn 사용 추천)
	// Deferred Spawn을 쓰면 스폰 완료(FinishSpawning) 전에 변수를 세팅할 수 있습니다.
	ABase_Projectile* Projectile = GetWorld()->SpawnActorDeferred<ABase_Projectile>(
		ProjectileClass, 
		SpawnTransform, 
		GetOwningActorFromActorInfo(), 
		Cast<APawn>(GetOwningActorFromActorInfo()), 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Projectile)
	{
		// ⭐ 여기서 명세서를 배달부에게 쥐어줍니다!
		Projectile->DamageEffectSpecHandle = SpecHandle;

		// 스폰 완료
		Projectile->FinishSpawning(SpawnTransform);
	}
}


