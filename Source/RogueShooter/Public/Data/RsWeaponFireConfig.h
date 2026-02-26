// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RsWeaponFireConfig.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ROGUESHOOTER_API URsWeaponFireConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	// 1. 소켓 이름 (자유롭게 입력 가능! 태그 등록 불필요)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FName SocketName;

	// 2. (확장성) 이 소켓으로 쏠 때의 데미지 배율 (예: 보조무기는 0.5배)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float DamageMultiplier = 1.0f;

	// 3. (확장성) 이펙트 등 추가 가능
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UParticleSystem* MuzzleFlashFX;
};
