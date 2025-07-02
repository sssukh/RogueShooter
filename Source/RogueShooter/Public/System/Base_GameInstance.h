// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Base_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UBase_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UBase_GameInstance();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Name")
	FName CurrentMap;
};
