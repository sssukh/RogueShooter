// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ItemIcon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Item/ItemData.h"
#include "RogueShooter/RSEnumStruct.h"


UUW_ItemIcon::UUW_ItemIcon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_ItemIcon::SetItemData(const FInventorySlot& InInventorySlot)
{
	Icon->SetBrushFromTexture(InInventorySlot.ItemData->Icon);
	
	CountText->SetText(FText::FromString(FString::Printf(TEXT("%d"),InInventorySlot.Count)));
}
