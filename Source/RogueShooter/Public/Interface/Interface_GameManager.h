// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_GameManager.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_GameManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUESHOOTER_API IInterface_GameManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Game Manager")
	void TransferXP(int32 XP);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Game Manager")
	void OnPlayerDeath();	
};
