// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_StatRow.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RogueShooter/RSEnumStruct.h"

UUW_StatRow::UUW_StatRow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_StatRow::UpdateRow(FStatUIData StatUIData, float Value)
{
	Description->SetText(FText::Format(StatUIData.FormatString,StatUIData.DisplayName,Value));
	if (StatUIData.Icon)
		StatIcon->SetBrushFromTexture(StatUIData.Icon);
}
