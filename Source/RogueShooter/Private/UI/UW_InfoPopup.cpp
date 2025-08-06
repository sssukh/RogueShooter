// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InfoPopup.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UUW_InfoPopup::UUW_InfoPopup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_InfoPopup::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_PopupText->SetText(DescriptionText);

	Button_Yes->OnClicked.AddDynamic(this,&UUW_InfoPopup::OnButtonYesClicked);
}

void UUW_InfoPopup::OnButtonYesClicked()
{
	if(OnYesClicked.IsBound())
		OnYesClicked.Broadcast();

	RemoveFromParent();
}
