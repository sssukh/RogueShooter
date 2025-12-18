// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InventoryMain.generated.h"

class UItemData;
class UUW_ItemTooltip;
class UUW_ItemIcon;
class UInventoryComponent;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_InventoryMain : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_InventoryMain(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void RefreshInventory();
	
	void InitInventory(UInventoryComponent* InventoryComponent);
	
	void ShowItemTooltip(const UItemData* ItemData);
	
	void HideItemTooltip();
public:
	UPROPERTY(BlueprintReadWrite,Category = "Inventory Main | Designer", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;
	
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> PlayerInventoryComponent;
	
	UPROPERTY()
	TObjectPtr<UUW_ItemTooltip> ItemTooltip;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UUW_ItemIcon> ItemIconClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<UUW_ItemTooltip> ItemTooltipClass;
};
