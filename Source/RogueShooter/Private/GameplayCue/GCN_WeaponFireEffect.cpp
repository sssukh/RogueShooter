// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/GCN_WeaponFireEffect.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/Base_Character.h"
#include "Components/RsWeaponComponent.h"
#include "Kismet/GameplayStatics.h"

UGCN_WeaponFireEffect::UGCN_WeaponFireEffect()
{
}

bool UGCN_WeaponFireEffect::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (!MyTarget) return false;
	
	ABase_Character* Character = Cast<ABase_Character>(MyTarget);
	
	if (!Character) return false;
	
	// 입력된 GameplayEffectLevel(muzzle index)가 배열 범위를 벗어나면 return
	if (Character->WeaponComponent->MuzzleSocketNames.Num()<Parameters.GameplayEffectLevel)
		return false;
	
	int32 MuzzleIndex = Parameters.GameplayEffectLevel;
	
	if (bUseMuzzleFlash && NiagaraMuzzleFlash)
	{
	
		UNiagaraComponent* CreatedMuzzleFlash = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraMuzzleFlash,Character->GetMesh(),
			Character->WeaponComponent->GetMuzzleNameByIndex(MuzzleIndex),FVector(),
			FRotator(0.0f,90.0f,0.0f),EAttachLocation::Type::KeepRelativeOffset,
			true,true,ENCPoolMethod::AutoRelease,true);
		
		if (CreatedMuzzleFlash)
			OnSetMuzzleFlashEffectVariables(CreatedMuzzleFlash,Parameters);
	}	
	
	if (bUseTracer && NiagaraTracer)
	{
		UNiagaraComponent* CreatedTracer = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget,NiagaraTracer,Parameters.Location,
			FRotator(0.0f,90.0f,0.0f),FVector(1),true,true,ENCPoolMethod::AutoRelease,true);
		
		if (CreatedTracer)
			OnSetTracerEffectVariables(CreatedTracer,Parameters);
	}
	
	if (bUseSound)
	{
		UGameplayStatics::SpawnSoundAttached(FireSound,Character->GetMesh());
	}
	
	return true;
}
