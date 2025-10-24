// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MyCheatManager.h"

#include "Kismet/GameplayStatics.h"
#include "System/GameManager.h"
#include "Utility/RSLog.h"



void UMyCheatManager::Debug_Levelup()
{
	
	if(AGameManager* GameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AGameManager::StaticClass())))
	{
		RS_LOG_SCREEN(TEXT("Cheat Manager : LevelUp"))
		GameManager->PrepareLevelUp();
	}
}
