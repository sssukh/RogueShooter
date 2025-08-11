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

USTRUCT()
struct FAABIntPair
{
	GENERATED_BODY()
	UPROPERTY()
	EActiveAbilities AAbility;
	UPROPERTY()
	int32 Value;
};

USTRUCT()
struct FPABIntPair
{
	GENERATED_BODY()
	UPROPERTY()
	EPassiveAbilities PAbility;
	UPROPERTY()
	int32 Value;
};

UCLASS()
class ROGUESHOOTER_API AGameplay_PlayerController : public ABase_PlayerController, public IInterface_ControllerManager
{
	GENERATED_BODY()
public:
	AGameplay_PlayerController();

	virtual void BeginPlay() override;

	// Handles Level up UI Flow
	UFUNCTION(Client,Unreliable)
	void OC_CreateLevelupUI();

	// Handles Chest UI Flow
	UFUNCTION(Client,Unreliable)
	void OC_CreateChestUI();

	// Handles GameMenu Focus
	virtual void OnPauseMenuCalled() override;

	// Handles GameMenu Focus
	virtual void OnPauseMenuClosed() override;
	
	//******************************************************
	//	IInterface_ControllerManager
	//******************************************************
	// Hot bar
	virtual void UpdateHudHotbar_Implementation(TMap<EActiveAbilities, int32>& Active,TMap<EPassiveAbilities, int32>& Passive) override; 

	// Level bar
	virtual void UpdateLevelBar_Implementation(float Percent, int32 Level) override;

	// Timer
	virtual void UpdateTime_Implementation(FText& Time) override;

	// Gold
	virtual void UpdateGold_Implementation(int32 Amount) override;

	// End of game
	virtual void OnMatchEnd_Implementation(bool Victory, int32 EnemiesKilled) override;

	// Handles Level Up UI Flow
	virtual void OnLevelUp_Implementation() override;

	// Handles Chest UI flow
	virtual void OnChestFound_Implementation() override;
	
	//******************************************************
	//	Widget Updates
	//******************************************************
	
	// Level bar
	UFUNCTION(Client,Reliable)
	void OC_UpdateCharUI(float Percent, int32 Level);

	// 본 블루프린트에는 replicate 설정이 안되어있는데 함수 이름이 OC라서 일단 replicate 설정 해둠
	// Timer
	UFUNCTION(Client,Reliable)
	void OC_UpdateTimer(const FText& Time);

	// Hot bar
	UFUNCTION(Client,Unreliable)
	void OC_UpdateHudHotbar(const TArray<FAABIntPair>& Active,const TArray<FPABIntPair>& Passive);

	// Gold
	UFUNCTION(Client,Unreliable)
	void OC_UpdateGoldInUI(int32 Amount);

	// End of game
	UFUNCTION(Client,Unreliable)
	void OC_EndMatch(bool Victory, int32 EnemiesKilled);
	//******************************************************
	//	Setup
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

	void PrepareChest();
	
	/**
	 * Random Roll to determine how many items to have in chest
	 * @return 
	 */
	int32 DetermineChestCount();

	void BuildAndProcessChest(int32 index);

	TArray<EActiveAbilities> Chest_BuildActiveList(TMap<EActiveAbilities,int32> ActiveMap,int32 MaxLevel);

	TArray<EPassiveAbilities> Chest_BuildPassiveList(TMap<EPassiveAbilities,int32> PassiveMap, int32 MaxLevel);
	
	UFUNCTION()
	void CloseChestUI();

	
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

	

	UPROPERTY()
	TObjectPtr<UDataTable> DT_ActiveAbilities;

	UPROPERTY()
	TObjectPtr<UDataTable> DT_PassiveAbilities;
	
	UPROPERTY()
	TObjectPtr<UTexture2D> HealthPotionIcon;

	UPROPERTY()
	TObjectPtr<UTexture2D> CoinIcon;

	UPROPERTY()
	TObjectPtr<USoundBase> WinSound;

	UPROPERTY()
	TObjectPtr<USoundBase> LoseSound;
	
	TSubclassOf<UUW_PlayerHud> PlayerHudClass;

	TSubclassOf<UUW_LevelUpMaster> LevelUpMasterClass;

	TSubclassOf<UUW_MatchResults> MatchResultClass;

	TSubclassOf<UUW_ChestMaster> ChestMasterClass;
};
