// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_PlayerState.h"
#include "Base_PlayerState.generated.h"


class USG_Player;
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

	UFUNCTION()
	void OnRep_GoldCount();
	
	UFUNCTION(Client,Unreliable)
	void OC_GoldPickUp(int32 Amount);
	
	UFUNCTION(Client,Unreliable)
	void OC_Save();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Interface

	virtual void CallGameSave_Implementation() override;

	virtual void OnGoldPickUp_Implementation(int32 Amount) override;
public:
	UPROPERTY(ReplicatedUsing=OnRep_GoldCount,EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	int32 GoldCount = 0 ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	TObjectPtr<USG_Player> GameSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="PlayerState")
	int32 TempGold = 0 ;
};
