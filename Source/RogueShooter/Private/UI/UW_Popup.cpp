// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_Popup.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UUW_Popup::UUW_Popup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_Popup::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_PopupText->SetText(DescriptionText);

	Button_No->OnClicked.AddDynamic(this,&UUW_Popup::OnButtonNoClicked);

	Button_Yes->OnClicked.AddDynamic(this,&UUW_Popup::OnButtonYesClicked);
}

void UUW_Popup::OnButtonYesClicked()
{
	if(OnYesClicked.IsBound())
		OnYesClicked.Broadcast();

	RemoveFromParent();
}

void UUW_Popup::OnButtonNoClicked()
{
	if(OnNoClicked.IsBound())
		OnNoClicked.Broadcast();

	RemoveFromParent();
}
