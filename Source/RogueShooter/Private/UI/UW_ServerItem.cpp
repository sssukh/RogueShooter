// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ServerItem.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_LoadingScreen.h"
#include "OnlineSubsystem.h"

UUW_ServerItem::UUW_ServerItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUW_ServerItem::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Name->SetText(FText::FromString(SessionResult.Session.OwningUserName));

	int32 MaxPlayers = SessionResult.Session.SessionSettings.NumPublicConnections;
	int32 CurrentPlayers = MaxPlayers - SessionResult.Session.NumOpenPublicConnections;
	TextBlock_Players->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"),CurrentPlayers,MaxPlayers)));

	Button_Server->OnClicked.AddDynamic(this,&UUW_ServerItem::OnButtonServerClicked);
}

void UUW_ServerItem::OnButtonServerClicked()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	if(SessionsSubsystem)
	{
		SessionsSubsystem->JoinSession(SessionResult);

		UFunctionLibrary_Helper::CreateLoadingScreen(GetWorld(),FText::FromString(TEXT("Joining Session")),LoadingScreenClass);
	}
}
