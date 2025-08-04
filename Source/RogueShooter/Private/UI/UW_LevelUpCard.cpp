// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LevelUpCard.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UUW_LevelUpCard::UUW_LevelUpCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_LevelUpCard::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this,&UUW_LevelUpCard::ButtonOnClicked);

	Image_Icon->SetBrushFromTexture(Icon);

	TextBlock_Name->SetText(Name);

	FText LevelName;

	if(Level == 0 || Level == 1)
		LevelName = FText::FromString(TEXT("New!"));
	else
	{
		LevelName = FText::FromString(FString::Printf(TEXT("Level : %d"),Level));
	}
	
	TextBlock_Level->SetText(LevelName);

	TextBlock_Desc->SetText(Description);

	if(Level == -1)
	{
		TextBlock_Level->SetText(FText::FromString(TEXT("Evolution")));
	}
}

void UUW_LevelUpCard::ButtonOnClicked()
{
	if(OnSelected.IsBound())
		OnSelected.Broadcast(Type,AAbility,PAbility);
}
