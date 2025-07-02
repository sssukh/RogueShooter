// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base_PlayerState.h"

#include "Interface/Interface_ControllerManager.h"

ABase_PlayerState::ABase_PlayerState()
{
}

void ABase_PlayerState::OnRep_GoldCount()
{
	IInterface_ControllerManager::Execute_UpdateGold(GetPlayerController(),GoldCount);
}
