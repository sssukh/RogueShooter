// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemIcon.generated.h"

class UUW_InventoryMain;
struct FInventorySlot;
class UTextBlock;
class UItemData;
class UImage;
/**
 * 인벤토리 슬롯에서 보여지는 아이콘 위젯.
 */
UCLASS()
class ROGUESHOOTER_API UUW_ItemIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ItemIcon(const FObjectInitializer& ObjectInitializer);
	
	// Refresh Inventory를 할 때 슬롯의 데이터인 이미지와 개수를 적용. 
	void SetItemData(const FInventorySlot& InItemData);
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
public:
	UPROPERTY(BlueprintReadWrite,Category = "Item Icon | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(BlueprintReadWrite,Category = "Item Icon | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;
	
	UPROPERTY()
	TObjectPtr<UUW_InventoryMain> InventoryMain;
	
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TObjectPtr<UItemData> CachedItemData;
};
