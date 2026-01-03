// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ItemTooltip.generated.h"

class UUW_StatRow;
struct FGameplayTag;
struct FStatUIData;
class UItemData;
class UVerticalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ItemTooltip(const FObjectInitializer& ObjectInitializer);
	
	
	void UpdateTooltip(const UItemData* ItemData, int32 ItemCount);

	/**
	 * 
	 * @param Tag 찾는데 사용할 GameplayTag
	 * @return Tag를 이용해 찾은 StatUIData
	 */
	FStatUIData* FindStatUIDataByTag(FGameplayTag Tag);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ItemTooltip | Designer ",meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ItemTooltip | Designer ",meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ItemTooltip | Designer ",meta = (BindWidget))
	TObjectPtr<UVerticalBox> Statlist; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TObjectPtr<UDataTable> StatDataTable;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "Config")
	TSubclassOf<UUW_StatRow> StatRowWidgetClass;
};
