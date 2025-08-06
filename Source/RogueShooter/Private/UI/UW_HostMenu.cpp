// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HostMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UUW_HostMenu::UUW_HostMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_HostMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Host->OnClicked.AddDynamic(this,&UUW_HostMenu::OnButtonCloseClicked);

	Button_Close->OnClicked.AddDynamic(this,&UUW_HostMenu::OnButtonHostClicked);
}

void UUW_HostMenu::OnButtonHostClicked()
{
	// TODO : CreateSession
}

void UUW_HostMenu::OnButtonCloseClicked()
{
	RemoveFromParent();
}
