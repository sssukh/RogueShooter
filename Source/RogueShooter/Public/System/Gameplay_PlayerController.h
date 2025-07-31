// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_PlayerController.h"
#include "Interface/Interface_ControllerManager.h"
#include "Gameplay_PlayerController.generated.h"

class UUW_PlayerHud;
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

	//******************************************************
	//*	Setup
	//******************************************************
	
	UFUNCTION()
	void SetupPlayer();

	/**
	 * Create Player Hud
	 */
	void CreateGameplayUI();

	/**
	 * Set References for later use
	 */
	void SetReference();

	//******************************************************
	// Widget Manager
	//******************************************************

	// LevelUp

	void PrepareLevelUp();

	UFUNCTION()
	void ExecuteLevelUp();

	bool CheckIfEvoReady(EActiveAbilities& Ability);

	TArray<EActiveAbilities> CheckActiveAbilities(TMap<EActiveAbilities,int32> ActiveMap, int32 MaxLevel);

	TArray<EPassiveAbilities> CheckPassiveAbilities(TMap<EPassiveAbilities,int32> PassiveMap, int32 MaxLevel);
	
	UFUNCTION()
	void ProcessLevelUp(EAbilityType Type, EActiveAbilities AAbilities, EPassiveAbilities PAbilities);
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

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UUW_PlayerHud> PlayerHud;

	// TODO : 초기설정
	TSubclassOf<UUW_PlayerHud> PlayerHudClass;

	// TODO : 초기설정
	TSubclassOf<UUW_LevelUpMaster> LevelUpMasterClass;

	// TODO : 초기설정 
	UPROPERTY()
	TObjectPtr<UDataTable> DT_ActiveAbilities;

	// TODO : 초기설정 
	UPROPERTY()
	TObjectPtr<UDataTable> DT_PassiveAbilities;
	
	// TODO : 초기설정
	UPROPERTY()
	TObjectPtr<UTexture2D> HealthPotionIcon;

	// TODO : 초기설정
	UPROPERTY()
	TObjectPtr<UTexture2D> CoinIcon;
};
