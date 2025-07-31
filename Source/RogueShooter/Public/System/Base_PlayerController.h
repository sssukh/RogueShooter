// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Base_PlayerController.generated.h"

class UUW_GameMenu;
class UWidget;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ABase_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABase_PlayerController();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// TODO : ESC, P 키 눌렀을 때 게임 메뉴가 나타나도록 하는 함수 구현 필요

	// void OpenMenu();
	
	// remove all UI
	virtual void CleanUpUI();

	// override for when pause menu called
	virtual void OnPauseMenuCalled();

	// override for when pause menu closed
	virtual void OnPauseMenuClosed();
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerController")
	TObjectPtr<UUW_GameMenu> GameMenu;
};
