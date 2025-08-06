// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_CharacterSelectionItem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Library/FunctionLibrary_Helper.h"
#include "Saves/SG_Player.h"
#include "UI/UW_InfoPopup.h"

UUW_CharacterSelectionItem::UUW_CharacterSelectionItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_CharacterSelectionItem::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterName = CharData.CharacterName;

	TextBlock_Character->SetText(CharacterName);

	TextBlock_Description->SetText(Description);

	if(!Locked)
		return;

	Button->SetStyle(LockedStyle);

	TextBlock_Description->SetText(FText::FromString(FString::Printf(TEXT("%s\nUnlock Cost : %d"),*Description.ToString() ,Cost)));

	Button->OnClicked.AddDynamic(this,&UUW_CharacterSelectionItem::OnButtonClicked);
}

void UUW_CharacterSelectionItem::PurchaseFlow()
{
	if(IsValid(SaveData))
	{
		SaveData = UFunctionLibrary_Helper::LoadPlayerData(GetWorld());
	}

	if(SaveData->Gold < Cost)
	{
		if(UUW_InfoPopup* InfoPopup = CreateWidget<UUW_InfoPopup>(GetOwningPlayer(),InfoPopupClass))
		{
			InfoPopup->DescriptionText = FText::FromString(TEXT("Not Enough Gold"));

			InfoPopup->AddToViewport();
		}
		return;
	}

	if(UUW_InfoPopup* InfoPopup = CreateWidget<UUW_InfoPopup>(GetOwningPlayer(),InfoPopupClass))
	{
		InfoPopup->DescriptionText = FText::FromString(FString::Printf(TEXT("Unlock %s for %d gold?"),*CharacterName.ToString(),Cost));
		
		InfoPopup->AddToViewport();
		
		InfoPopup->OnYesClicked.AddDynamic(this,&UUW_CharacterSelectionItem::PurchaseCharacter);
	}

		
}

void UUW_CharacterSelectionItem::PurchaseCharacter()
{
	if(OnPurchase.IsBound())
		OnPurchase.Broadcast(CharData);
}

void UUW_CharacterSelectionItem::OnButtonClicked()
{
	if(!Locked)
	{
		if(OnClicked.IsBound())
			OnClicked.Broadcast(CharData);
			
	}
	else
	{
		PurchaseFlow();
	}
}
