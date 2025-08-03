// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_PlayerController.h"
#include "Interface/Interface_ControllerManager.h"
#include "Gameplay_PlayerController.generated.h"

class UUW_MatchResults;
class UUW_PlayerHud;
class UUW_LevelUpMaster;
class UUW_ChestMaster;
class UAbilitiesComponent;
/**
 * 
 */

ENUM_RANGE_BY_COUNT(EActiveAbilities,EActiveAbilities::MAX)
ENUM_RANGE_BY_COUNT(EPassiveAbilities,EPassiveAbilities::MAX)

UCLASS()
class ROGUESHOOTER_API AGameplay_PlayerController : public ABase_PlayerController, public IInterface_ControllerManager
{
	GENERATED_BODY()
public:
	AGameplay_PlayerController();
	//******************************************************
	//	IInterface_ControllerManager
	//******************************************************
	virtual void UpdateHudHotbar_Implementation(const TMap<EActiveAbilities, int32>& Active, const TMap<EPassiveAbilities, int32>& Passive) override; 
	
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
	/**
	 * Update Character XP and Level
	 * @param Percent Xp percentage
	 * @param Level Player Level
	 */
	void UpdateCharacterUI(float Percent, int32 Level);

	/**
	 * Refresh Hotbar with active and passive skills
	 */
	void UpdateHotbar();

	/**
	 * Show UI when match has ended
	 * @param Victory boolean for victory
	 * @param EnemiesKilled the number of enemies killed
	 */
	void ShowEndMatchScreen(bool Victory, int32 EnemiesKilled);
	
	// LevelUp

	void PrepareLevelUp();

	UFUNCTION()
	void ExecuteLevelUp();

	/**
	 * Search through evo array to see if pair is found\n
	 * 진화 배열을 탐색해서 진화 쌍이 존재하는지 확인한다.
	 * @param Ability Ability to Evolute
	 * @return boolean for Evolution ready
	 */
	bool CheckIfEvoReady(EActiveAbilities& Ability);

	TArray<EActiveAbilities> CheckActiveAbilities(TMap<EActiveAbilities,int32> ActiveMap, int32 MaxLevel);

	TArray<EPassiveAbilities> CheckPassiveAbilities(TMap<EPassiveAbilities,int32> PassiveMap, int32 MaxLevel);
	
	UFUNCTION()
	void ProcessLevelUp(EAbilityType Type, EActiveAbilities AAbilities, EPassiveAbilities PAbilities);

	// Chest





	
	//******************************************************
	// Abilities
	//******************************************************

	/**
	 * Save Evolution information in an array to fetch later
	 * @param Evo Active Ability to evolute later
	 */
	void ActivateEvolution(EActiveAbilities Evo);
	/**
	 * Based on levelup or chest - level up ability on pawn component
	 * @param Type ability type
	 * @param PAbility passive ability
	 * @param AAbility active ability
	 */
	void AssignAbility(EAbilityType Type, EPassiveAbilities PAbility, EActiveAbilities AAbility);
	
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

	// TODO : 초기설정
	UPROPERTY()
	TObjectPtr<USoundBase> WinSound;

	// TODO : 초기설정
	UPROPERTY()
	TObjectPtr<USoundBase> LoseSound;
	
	// TODO : 초기설정
	TSubclassOf<UUW_PlayerHud> PlayerHudClass;

	// TODO : 초기설정
	TSubclassOf<UUW_LevelUpMaster> LevelUpMasterClass;

	// TODO : 초기설정 
	TSubclassOf<UUW_MatchResults> MatchResultClass;
};
