// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_PlayerState.h"
#include "Base_PlayerState.generated.h"


// Event_ 는 interface로 구현하면 된다.
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ABase_PlayerState : public APlayerState, public IInterface_PlayerState
{
	GENERATED_BODY()
public:
	ABase_PlayerState();

	void OnRep_GoldCount();

	// TODO :  아래 이벤트들은 Owning Client로 replicate된 것들이다.
	// 뭔지 일단 알아야겠다.
	// OC_Save
	// OC_GoldPickup
	
public:
	// Interface

	// virtual void CallGameSave_Implementation() override;

	// virtual void OnGoldPickUp_Implementation(int32 Amount) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	int32 GoldCount = 0 ;

	// TODO : SG_Player 구현 필요 
	// UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	// TObjectPtr<SG_Player> GameSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	int32 TempGold = 0 ;
};
