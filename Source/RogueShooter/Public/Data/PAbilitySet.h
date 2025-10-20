// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PAbilitySet.generated.h"

enum class EPassiveAbilities : uint8;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UPAbilitySet : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Database")
	TMap<EPassiveAbilities,TSoftObjectPtr<UTexture2D>> PAbilityIcon;
};
