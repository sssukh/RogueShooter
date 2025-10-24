// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/Interface_GameManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyGameManager.generated.h"

struct FEnemySpawns;
struct FEnemySpawnType;
class AGameplay_PlayerController;
class ABase_Character;
class UEnvQueryInstanceBlueprintWrapper;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UMyGameManager : public UWorldSubsystem, public IInterface_GameManager
{
	GENERATED_BODY()

	
public:
	// Sets default values for this actor's properties
	UMyGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void Initialize() ;

public:
	// Handle UI XP update
	void UpdateCharactersXP(float pPercent, int32 pLevel);

	virtual void OnPlayerDeath_Implementation() override;
	
	void EndGame(bool Victory);

	// Save Player Data
	void SavePlayerData();

	/////////////////////////////////////
	/// Spawn System
	/////////////////////////////////////

	void SpawnWave();

	// Ensure we are not over spawn limits
	void ContinueSpawning();

	UFUNCTION()
	void DecreaseEnemyCount();

	void IncreaseEnemyCount();

	// Spawn in Elite Array
	void PrepareEliteSpawn();
	
	/////////////////////////////////////
	/// Character Logic
	/////////////////////////////////////

	/**
	 *	Create an Xp array that stores how much Xp is needed for each level
	 */
	void CreateXPTable();

	/**
	 *	return xp need to next level
	 * @return xp need to next level
	 * 
	 */
	int32 XPToNextLevel();

	/**
	 * add xp to tracker and update UI
	 * 
	 */
	void AddXp(int32 XP);

	/**
	 *	Prepare UI for Level up
	 * 
	 */
	void PrepareLevelUp();

	virtual void TransferXP_Implementation(int32 XP) override;
	
	/////////////////////////////////////
	/// Setup
	/////////////////////////////////////

	void GameSetup();

	void SpawnSetup();

	void PrepareWaveEnemies();

	// build an array with references to player and controller;
	// on player characters, set an interface reference to GM, to avoid circular references
	void BuildPlayerArray();

	void SetReferences();

	void StartTimer();

	void PrepareWaveElites();


	/////////////////////////////////////
	/// GameLogic
	/////////////////////////////////////

	UFUNCTION()
	void SpawnEnemy(UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status);

	void FindSpawnLocation();

	// On Player death - see if anyone else is alive - if so - keep match running
	void DetemineGameStatus();

	void UpdateTimer();

	void ProcessEndGame();

	void IncreaseWaveIndex();

	UFUNCTION()
	void SpawnElite(UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status);

	// Check if the wave index has enemies assigned - if not - use previous saved enemies
	void UpdateEnemyWave();

	// send game time to every player
	UFUNCTION()
	void OnRep_GameTime();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void SetGameTimeOnServer(FString& pSecond);
	
public:
	/////////////////////////////////////
	/// Member Variables
	/////////////////////////////////////
	
	// Character Logic
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Character Logic")
	int32 MaxLevel = 99;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Character Logic")
	TArray<int32> XPArray;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Character Logic")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Character Logic")
	int32 NeededXP;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Character Logic")
	int32 CurrentXP;

	// Timers
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Timers")
	FTimerHandle SpawnTimerReference;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Timers")
	FTimerHandle PhaseIndexTimer;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Timers")
	FTimerHandle ClockReference;

	// Game Logic
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	TArray<ABase_Character*> PlayerCharacterArray;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	TArray<AGameplay_PlayerController*> PlayerControllerArray;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	int32 EnemiesKilled;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	int32 Time;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	int32 Minutes;

	UPROPERTY(ReplicatedUsing="OnRep_GameTime",VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	FText GameTime;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	int32 MaxGameTime = 10;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Game Logic")
	float SpawnInterval =2.0f;

	// Spawn Logic
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 WaveIndex;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	TArray<FEnemySpawnType> PreparedEnemies;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 MaxEnemies = 45;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 CurrentEnemyCount;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 MaxEnemiesPerWave = 20;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 MinEnemiesPerWave = 10;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 NumberOfEnemiesToSpawn = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	int32 CurrentEnemySpawnIndex = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	TArray<FEnemySpawns> EnemySpawns;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic")
	TArray<FEnemySpawns> EliteSpawns;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic", meta = (ExposeOnSpawn))
	TObjectPtr<UDataTable> EnemySpawnDT;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameManager | Spawn Logic",meta = (ExposeOnSpawn))
	TObjectPtr<UDataTable> EnemyEliteSpawnDT;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="GameManager | Default")
	TObjectPtr<UEnvQuery> EQS_FindSpawnPoint;
};
