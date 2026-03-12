// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCN_WeaponFireEffect.generated.h"

class UNiagaraComponent;
/**
 * 총을 발사할 때 필요한 효과들(MuzzleFlash, Tracer, Sound)를 적용하는 GCN
 * bUse bool 변수들을 설정해서 필요한 것만 적용 가능하다.
 * 블루프린트에서 OnSet함수들을 통해 나이아가라에 필요한 변수 세팅을 해주어야한다.
 * 
 * 캐릭터의 weaponComponent에 MuzzleSocket이 등록되어있어야한다.
 */
UCLASS()
class ROGUESHOOTER_API UGCN_WeaponFireEffect : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UGCN_WeaponFireEffect();

	// 엔진이 GCN을 실행할 때 호출하는 핵심 함수 (const가 붙어있음에 주의!)
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

	
	
	// 생성된 나이아가라 컴포넌트를 BP로 넘겨주어 변수만 세팅하게 하는 이벤트
	// 주의: GCN 자체는 변하면 안 되므로 맨 뒤에 const가 반드시 있어야 합니다.
	
	// MuzzleFlashEffect의 나이아가라 변수 설정 
	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void OnSetMuzzleFlashEffectVariables(UNiagaraComponent* SpawnedNiagara, const FGameplayCueParameters& Parameters) const;
	
	// TracerEffect의 나이아가라 변수 설정 
	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void OnSetTracerEffectVariables(UNiagaraComponent* SpawnedNiagara, const FGameplayCueParameters& Parameters) const;
protected:
	// 블루프린트 자식 클래스에서 이펙트 에셋을 넣을 수 있게 열어둡니다.
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UNiagaraSystem* NiagaraMuzzleFlash; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UNiagaraSystem* NiagaraTracer; 
	
	// 만약 Cascade 파티클을 쓴다면 UParticleSystem* 을 사용하세요.

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class USoundBase* FireSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	bool bUseMuzzleFlash = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	bool bUseTracer = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	bool bUseSound = true;
};
