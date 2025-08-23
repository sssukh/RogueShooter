// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/Interface_ControllerManager.h"
#include "System/Base_PlayerController.h"
#include "Lobby_PlayerController.generated.h"

class UUW_LobbyMenu;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ALobby_PlayerController : public ABase_PlayerController, public IInterface_ControllerManager
{
	GENERATED_BODY()
public:
	ALobby_PlayerController();

	virtual void BeginPlay() override;

	// Interface

	virtual void OnPlayerJoin_Implementation() override;

	virtual void OnMatchEnd_Implementation(bool Victory, int32 EnemiesKilled) override;

	virtual void UpdateLevelBar_Implementation(float Percent, int32 Level) override;
	
	UFUNCTION(Client,Unreliable)
	void OC_UpdatePlayerTotal();

	// Member Function
	void CreateUI();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UUW_LobbyMenu> PlayerHud;

	TSubclassOf<UUW_LobbyMenu> LobbyMenuClass;
};
