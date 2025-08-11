// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base_PlayerController.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

ABase_PlayerController::ABase_PlayerController()
{
}

void ABase_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// input mode 설정 
	FInputModeGameOnly InputModeGameOnly;
	
	SetInputMode(InputModeGameOnly);
}

void ABase_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (EndPlayReason)
	{
	case(EEndPlayReason::Type::RemovedFromWorld):
	case(EEndPlayReason::Type::Quit):
		{
			UGameplayStatics::OpenLevel(this,FName("M_MainMenu"));
			
			UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());

			UMultiplayerSessionsSubsystem* SessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();

			SessionsSubsystem->DestroySession();
		}
		
	}
}

void ABase_PlayerController::CleanUpUI()
{
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
}

void ABase_PlayerController::OnPauseMenuCalled()
{
}

void ABase_PlayerController::OnPauseMenuClosed()
{
}
