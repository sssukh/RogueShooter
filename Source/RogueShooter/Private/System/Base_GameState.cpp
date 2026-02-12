// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base_GameState.h"

#include "Interface/Interface_ControllerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ABase_GameState::ABase_GameState()
{
	bReplicates =  true;
}

void ABase_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABase_GameState, GameTime);
	DOREPLIFETIME(ABase_GameState, EnemiesKilled);
	DOREPLIFETIME(ABase_GameState, Level);
	DOREPLIFETIME(ABase_GameState, CurrentXP);
	DOREPLIFETIME(ABase_GameState, NeededXP);
}

void ABase_GameState::IncreaseEnemyKill()
{
	++EnemiesKilled;
}

void ABase_GameState::OnRep_GameTime()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Controller && Controller->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
	{
		// IInterface_ControllerManager::Execute_UpdateTime(Controller, GameTime);
	}
}

void ABase_GameState::UpdateGameTime(int32 Minutes, int32 Seconds)
{
	if (HasAuthority())
	{
		FString SecondStr = Seconds < 10 ? FString::Printf(TEXT("0%d"), Seconds) : FString::Printf(TEXT("%d"), Seconds);
		GameTime = FText::FromString(FString::Printf(TEXT("%d:%s"), Minutes, *SecondStr));
        
		// 서버에서도 UI 갱신이 필요하다면 여기서 OnRep 호출 혹은 직접 로직 수행
		OnRep_GameTime(); 
	}
}
