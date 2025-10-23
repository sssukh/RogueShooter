// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MyCheatManager.generated.h"

class AGameManager;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	UFUNCTION(Exec)
	void Debug_Levelup();


};
