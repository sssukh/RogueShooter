// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MyCheatManager.h"

#include "System/GameManager.h"
#include "Utility/RSLog.h"



void UMyCheatManager::Debug_Levelup()
{
	if(GameManager)
	{
		RS_LOG_SCREEN(TEXT("Cheat Manager : LevelUp"))
		GameManager->PrepareLevelUp();
	}
}
