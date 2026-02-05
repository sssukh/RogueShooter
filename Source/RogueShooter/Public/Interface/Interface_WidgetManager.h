// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_WidgetManager.generated.h"

class URsBaseWidgetController;
// This class does not need to be modified.
UINTERFACE()
class UInterface_WidgetManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUESHOOTER_API IInterface_WidgetManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Widget Manager")
	void SetWidgetController(URsBaseWidgetController* InWidgetController);
};
