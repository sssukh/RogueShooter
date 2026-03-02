// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RsCharacterInfo.generated.h"

class ABase_Character;
class UUW_ReticleWidgetBase;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API URsCharacterInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<ABase_Character> CharacterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUW_ReticleWidgetBase> ReticleWidgetClass;
	
};
