// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_MainMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_HostMenu.h"
#include "UI/UW_ServerBrowser.h"

UUW_MainMenu::UUW_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UUW_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Host->OnClicked.AddDynamic(this,&UUW_MainMenu::OnButtonHostClicked);

	Button_Exit->OnClicked.AddDynamic(this,&UUW_MainMenu::OnButtonExitClicked);

	Button_Exit->OnClicked.AddDynamic(this,&UUW_MainMenu::OnButtonJoinClicked);
}

void UUW_MainMenu::OnButtonHostClicked()
{
	if(UUW_HostMenu* HostMenu = CreateWidget<UUW_HostMenu>(GetOwningPlayer(),HostMenuClass))
	{
		HostMenu->AddToViewport();
	}
}

void UUW_MainMenu::OnButtonExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}

void UUW_MainMenu::OnButtonJoinClicked()
{
	if(UUW_ServerBrowser* ServerBrowser = CreateWidget<UUW_ServerBrowser>(GetOwningPlayer(),ServerBrowserClass))
	{
		ServerBrowser->AddToViewport();
	}
}
