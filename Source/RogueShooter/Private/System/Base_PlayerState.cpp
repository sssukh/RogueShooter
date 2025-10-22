// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base_PlayerState.h"

#include "Interface/Interface_ControllerManager.h"
#include "Library/FunctionLibrary_Helper.h"
#include "Net/UnrealNetwork.h"
#include "Saves/SG_Player.h"

ABase_PlayerState::ABase_PlayerState()
{
	bReplicates = true;
}

void ABase_PlayerState::OnRep_GoldCount()
{
	if(GetPlayerController()->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
	{
		IInterface_ControllerManager::Execute_UpdateGold(GetPlayerController(),GoldCount);
	}
}

void ABase_PlayerState::OC_GoldPickUp_Implementation(int32 Amount)
{
	// For UI
	GoldCount = GoldCount+Amount;

	if(HasAuthority())
	
	// For Save
	TempGold = TempGold + Amount;

}

void ABase_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABase_PlayerState,GoldCount);
}

void ABase_PlayerState::CallGameSave_Implementation()
{
	// IInterface_PlayerState::CallGameSave_Implementation();
	OC_Save();
}

void ABase_PlayerState::OnGoldPickUp_Implementation(int32 Amount)
{
	// IInterface_PlayerState::OnGoldPickUp_Implementation(Amount);
	OC_GoldPickUp(Amount);

	if(HasAuthority())
		OnRep_GoldCount();
}

void ABase_PlayerState::OC_Save_Implementation()
{
	GameSave = UFunctionLibrary_Helper::LoadPlayerData(GetWorld());

	GameSave->Gold +=TempGold;

	UFunctionLibrary_Helper::SavePlayerData(GetWorld(),GameSave);

	TempGold=0;
}
