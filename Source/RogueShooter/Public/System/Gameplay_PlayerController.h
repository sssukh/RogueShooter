// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_PlayerController.h"
#include "Interface/Interface_ControllerManager.h"
#include "Gameplay_PlayerController.generated.h"

class UUW_LevelUpMaster;
class UUW_ChestMaster;
class UAbilitiesComponent;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API AGameplay_PlayerController : public ABase_PlayerController, public IInterface_ControllerManager
{
	GENERATED_BODY()
public:
	AGameplay_PlayerController();

	UFUNCTION()
	void SetupPlayer();
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	TObjectPtr<UAbilitiesComponent> AbilityComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Ability")
	TObjectPtr<UUW_LevelUpMaster> LevelUpUI;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Ability")
	bool PreparingUI;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Ability")
	bool LevelUpHudUp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Ability")
	bool ChestHudUp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Ability")
	TObjectPtr<UUW_ChestMaster> ChestUI;

	// TODO : WB_PlayerHud 구현 필요
	// UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	// TObjectPtr<>

};
