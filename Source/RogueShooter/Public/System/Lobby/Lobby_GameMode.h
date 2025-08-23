// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base_GameMode.h"
#include "Lobby_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ALobby_GameMode : public ABase_GameMode
{
	GENERATED_BODY()
public:
	ALobby_GameMode();

	virtual void OnPostLogin(AController* NewPlayer) override;

	void UpdateLobbyUI();

	void UpdateClassNames();
	
};
