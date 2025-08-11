// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_GameMenu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UUW_GameMenu::UUW_GameMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_GameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Continue->OnClicked.AddDynamic(this,&UUW_GameMenu::OnClicked_Continue);
	Button_Quit_Desktop->OnClicked.AddDynamic(this,&UUW_GameMenu::OnClicked_QuitDesktop);
	Button_Quit_Menu->OnClicked.AddDynamic(this,&UUW_GameMenu::OnClicked_QuitMenu);
}

void UUW_GameMenu::OnClicked_Continue()
{
	RemoveFromParent();

	if(OnMenuClosed.IsBound())
		OnMenuClosed.Broadcast();
}

void UUW_GameMenu::OnClicked_QuitDesktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}

void UUW_GameMenu::OnClicked_QuitMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName(TEXT("M_MainMenu")));
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

	UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

	SessionsSubsystem->DestroySession();
}
