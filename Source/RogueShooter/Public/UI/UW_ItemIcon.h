// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemIcon.generated.h"

struct FInventorySlot;
class UTextBlock;
class UItemData;
class UImage;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ItemIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ItemIcon(const FObjectInitializer& ObjectInitializer);
	
	void SetItemData(const FInventorySlot& InItemData);
public:
	UPROPERTY(BlueprintReadWrite,Category = "Item Icon | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(BlueprintReadWrite,Category = "Item Icon | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;
};
