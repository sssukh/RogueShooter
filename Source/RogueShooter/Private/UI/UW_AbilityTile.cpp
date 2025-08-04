// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_AbilityTile.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Utility/RSLog.h"

UUW_AbilityTile::UUW_AbilityTile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_AbilityTile::NativeConstruct()
{
	Super::NativeConstruct();

	if(Icon == nullptr)
	{
		RS_LOG_ERROR(TEXT("Icon이 설정되지 않았습니다."))
	}
	Image_Icon->SetBrushFromTexture(Icon,false);

	TextBlock_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"),Level)));
}
