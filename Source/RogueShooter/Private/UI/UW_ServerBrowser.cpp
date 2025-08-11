// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ServerBrowser.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/CircularThrobber.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_ServerItem.h"
#include "Utility/RSLog.h"
#include "OnlineSessionSettings.h"

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

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	if(SessionsSubsystem)
	{
		FindSessionDelegateHandle = SessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this,&UUW_ServerBrowser::OnFindSession);
		
		SessionsSubsystem->FindSessions(SessionsToFind,LAN);
	}
}

void UUW_ServerBrowser::OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionSearchResults,
	bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		for(const FOnlineSessionSearchResult& SessionResult : SessionSearchResults)
		{
			if(UUW_ServerItem* ServerItem = CreateWidget<UUW_ServerItem>(GetOwningPlayer(),ServerItemClass))
			{
				ServerItem->SessionResult = SessionResult;
				
				ScrollBox_Servers->AddChild(ServerItem);
			}
			else
			{
				RS_LOG_ERROR(TEXT("ServerItem이 생성되지 않습니다."))
			}
		}

		CircularThrobber_Searching->SetVisibility(ESlateVisibility::Collapsed);

		TextBlock_Status->SetText(FText::FromString(TEXT("Refreshed")));
	}
	else
	{
		CircularThrobber_Searching->SetVisibility(ESlateVisibility::Collapsed);

		TextBlock_Status->SetText(FText::FromString(TEXT("No Servers Found")));
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	if(!SessionsSubsystem->MultiplayerOnFindSessionsComplete.Remove(FindSessionDelegateHandle))
	{
		RS_LOG_ERROR(TEXT("OnFindSessionComplete의 바인딩된 함수가 제거되지 않았습니다."))
	}
}
