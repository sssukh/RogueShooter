// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ServerBrowser.h"

#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UUW_ServerBrowser::UUW_ServerBrowser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_ServerBrowser::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Close->OnClicked.AddDynamic(this,&UUW_ServerBrowser::OnButtonCloseClicked);

	ComboBoxString_Type->OnSelectionChanged.AddDynamic(this,&UUW_ServerBrowser::OnComboBoxSelectionChanged);

	Button_Refresh->OnClicked.AddDynamic(this,&UUW_ServerBrowser::OnButtonRefreshClicked);

	RefreshServerList();
}

void UUW_ServerBrowser::OnButtonCloseClicked()
{
	RemoveFromParent();
}

void UUW_ServerBrowser::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	LAN = SelectedItem==FString::Printf(TEXT("LAN"));
}

void UUW_ServerBrowser::OnButtonRefreshClicked()
{
	RefreshServerList();
}

void UUW_ServerBrowser::RefreshServerList()
{
	CircularThrobber_Searching->SetVisibility(ESlateVisibility::Visible);

	TextBlock_Status->SetVisibility(ESlateVisibility::Visible);

	TextBlock_Status->SetText(FText());

	ScrollBox_Servers->ClearChildren();

	// FindSession
}
