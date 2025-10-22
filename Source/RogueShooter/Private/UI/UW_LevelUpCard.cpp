// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LevelUpCard.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Utility/RSLog.h"

UUW_LevelUpCard::UUW_LevelUpCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUW_LevelUpCard::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this,&UUW_LevelUpCard::ButtonOnClicked);

	// 아직 로드가 안되었다면 로딩중 이미지 설정 
	if(!bIsLoaded)
		Image_Icon->SetBrushFromTexture(LoadingIcon);

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



void UUW_LevelUpCard::OnIconLoaded_Internal(UObject* LoadedAsset)
{
	if(LoadedAsset)
	{
		bIsLoaded = true;
		Image_Icon->SetBrushFromTexture(Cast<UTexture2D>(LoadedAsset));
	}
	// 스피너 아이콘 제거?
}
