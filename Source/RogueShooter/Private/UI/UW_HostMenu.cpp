// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HostMenu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"
#include "UI/UW_LoadingScreen.h"

UUW_HostMenu::UUW_HostMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUW_HostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Host->OnClicked.AddDynamic(this,&UUW_HostMenu::OnButtonCloseClicked);

	Button_Close->OnClicked.AddDynamic(this,&UUW_HostMenu::OnButtonHostClicked);

	ComboBoxString_LaN->OnSelectionChanged.AddDynamic(this,&UUW_HostMenu::UUW_HostMenu::OnLanSelectionChanged);

	ComboBoxString_Players->OnSelectionChanged.AddDynamic(this,&UUW_HostMenu::OnPlayersSelectionChanged);
}

void UUW_HostMenu::OnButtonHostClicked()
{
	SetIsEnabled(false);
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	if(SessionsSubsystem)
	{
		SessionsSubsystem->CreateSession(MaxPlayers,FString(TEXT("Match")));
	
		SessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this,&UUW_HostMenu::OnCreateSession);
	}
	else
	{
		RS_LOG_ERROR(TEXT("MultiPlayer Session Subsystem을 가져올 수 없습니다."))
		return;
	}

	UFunctionLibrary_Helper::CreateLoadingScreen(GetWorld(),FText::FromString(FString::Printf(TEXT("Entering Lobby"))),LoadingScreenClass);
}

void UUW_HostMenu::OnButtonCloseClicked()
{
	RemoveFromParent();
}

void UUW_HostMenu::OnCreateSession(bool bWasSuccessful)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	if(SessionsSubsystem)
	{
		SessionsSubsystem->MultiplayerOnCreateSessionComplete.Remove(this,FName(TEXT("OnCreateSession")));
	}
	else
	{
		RS_LOG_ERROR(TEXT("MultiPlayer Session Subsystem을 가져올 수 없습니다."))
	}
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			if(LevelName.IsEmpty())
			{
				RS_LOG_ERROR(TEXT("LevelName을 설정해주세요"))
			}
			World->ServerTravel(FString::Printf(TEXT("%s?listen"),*LevelName));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}
		SetIsEnabled(true);
	}
}

void UUW_HostMenu::OnLanSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem == "LAN")
		LAN = true;
}

void UUW_HostMenu::OnPlayersSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	MaxPlayers = FCString::Atoi(*SelectedItem);
}
