// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LevelUpItems.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LevelUpItems : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LevelUpItems(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(BlueprintReadWrite,Category = "UUW_LevelUpItems | Designer", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Items;
};
