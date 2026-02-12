// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MyCheatManager.h"

#include "Kismet/GameplayStatics.h"
#include "System/GameManager.h"
#include "Utility/RSLog.h"



void UMyCheatManager::Debug_Levelup()
{
	
	// 임시 제거 
}

void UMyCheatManager::Debug_Kill()
{
	if (ABase_Character* Character = Cast<ABase_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0)))
	{
		IInterface_CharacterManager::Execute_CharDie(GetWorld(),nullptr);
	}
}
