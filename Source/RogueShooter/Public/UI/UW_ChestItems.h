// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ChestItems.generated.h"

class UVerticalBox;
class UTextBlock;
class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ChestItems : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ChestItems(const FObjectInitializer& ObjectInitializer);

	
public:
	UPROPERTY()
	TObjectPtr<UButton> Button_Close;

	UPROPERTY()
	TObjectPtr<UImage> Image;

	UPROPERTY()
	TObjectPtr<UTextBlock> TextBlock_Gold;

	UPROPERTY()
	TObjectPtr<UVerticalBox> VerticalBox_Items;
};
