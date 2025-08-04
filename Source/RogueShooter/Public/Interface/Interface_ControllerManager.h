// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../RSEnumStruct.h"
#include "Interface_ControllerManager.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_ControllerManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUESHOOTER_API IInterface_ControllerManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void UpdateHudHotbar(TMap<EActiveAbilities,int32>& Active, TMap<EPassiveAbilities,int32>& Passive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void UpdateGold(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void UpdateTime(FText& Time);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void OnChestFound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void OnLevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void UpdateLevelBar(float Percent, int32 Level);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void OnMatchEnd(bool Victory, int32 EnemiesKilled);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Controller Manager")
	void OnPlayerJoin();
};
