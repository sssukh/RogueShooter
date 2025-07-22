// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RogueShooter/RSEnumStruct.h"
#include "SG_Player.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API USG_Player : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 Gold = 0;

	UPROPERTY()
	TArray<FText> UnlockedCharacters;

	UPROPERTY()
	FAvailableCharacter Character;
};
