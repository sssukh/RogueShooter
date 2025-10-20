// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AAbilitySet.generated.h"

enum class EActiveAbilities : uint8;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UAAbilitySet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Database")
	TMap<EActiveAbilities,TSoftObjectPtr<UTexture2D>> AAbilityIcon;
};
