// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_MainMenu.h"

#include "Components/Button.h"
#include "UI/UW_HostMenu.h"

UUW_MainMenu::UUW_MainMenu(const FObjectInitializer ObjectInitializer) : Super()
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
	// TODO : QuitGame
}

void UUW_MainMenu::OnButtonJoinClicked()
{
	// TODO : ServerBrowser 구현 필요 
}
