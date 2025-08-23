// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenu_PlayerController.generated.h"

class UUW_MainMenu;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API AMainMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainMenu_PlayerController();

	virtual void BeginPlay() override;

public:
	TSubclassOf<UUW_MainMenu> MainMenuClass;
};
