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
	UPROPERTY(BlueprintReadWrite, Category = "UW_ChestItems | Designer", meta = (BindWidget))
	TObjectPtr<UButton> Button_Close;

	UPROPERTY(BlueprintReadWrite, Category = "UW_ChestItems | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite, Category = "UW_ChestItems | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Gold;

	UPROPERTY(BlueprintReadWrite, Category = "UW_ChestItems | Designer", meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Items;
};
