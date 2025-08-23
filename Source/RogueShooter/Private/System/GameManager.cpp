// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameManager.h"

#include "Components/AbilitiesComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "RogueShooter/AssetPath.h"
#include "System/Base_GameMode.h"
#include "System/Gameplay_PlayerController.h"
#include "Utility/RSLog.h"


// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemySpawnDTFinder(*AssetPath::DataTable::DT_EnemySpawns);
	if(EnemySpawnDTFinder.Succeeded())
		EnemySpawnDT = EnemySpawnDTFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyEliteSpawnDTFinder(*AssetPath::DataTable::DT_EnemyEliteSpawns);
	if(EnemyEliteSpawnDTFinder.Succeeded())
		EnemyEliteSpawnDT = EnemyEliteSpawnDTFinder.Object;

	static ConstructorHelpers::FObjectFinder<UEnvQuery> EQS_FSPFinder(*AssetPath::AI::EQS_FSP);
	if(EQS_FSPFinder.Succeeded())
		EQS_FindSpawnPoint = EQS_FSPFinder.Object;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		FTimerHandle Delay;
		GetWorldTimerManager().SetTimer(Delay,FTimerDelegate::CreateLambda([&]()
		{
			GameSetup();

			SpawnSetup();
		}),
		2.0f,
		false);
	}
	
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameManager::UpdateCharactersXP(float pPercent, int32 pLevel)
{
	for(AGameplay_PlayerController* PC : PlayerControllerArray)
	{
		if(PC->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
		{
			IInterface_ControllerManager::Execute_UpdateLevelBar(PC,pPercent,pLevel);
		}
	}
}

void AGameManager::OnPlayerDeath_Implementation()
{
	// IInterface_GameManager::OnPlayerDeath_Implementation();

	DetemineGameStatus();
}

void AGameManager::EndGame(bool Victory)
{
	ProcessEndGame();

	for(AGameplay_PlayerController* GPC : PlayerControllerArray)
	{
		if(GPC->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
		{
			IInterface_ControllerManager::Execute_OnMatchEnd(GPC,Victory,EnemiesKilled);
		}
	}

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([&]()
	{
		ABase_GameMode* GameMode = Cast<ABase_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->ServerTravel_Lobby();
	}),5.0f,false);
}

void AGameManager::SavePlayerData()
{
	for(AGameplay_PlayerController* GPC : PlayerControllerArray)
	{
		if(GPC->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
		{
			IInterface_PlayerState::Execute_CallGameSave(GPC);
		}
	}
}

void AGameManager::SpawnWave()
{
	// calculate how many enemies we want to spawn this interval
	UpdateEnemyWave();

	CurrentEnemySpawnIndex = 0;

	NumberOfEnemiesToSpawn = FMath::RandRange(MinEnemiesPerWave,MaxEnemiesPerWave);

	// ensure we are not over spawn limits
	ContinueSpawning();
}

void AGameManager::ContinueSpawning()
{
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([&]()
	{
		if(CurrentEnemyCount<MaxEnemies && CurrentEnemySpawnIndex < NumberOfEnemiesToSpawn)
		{
			// Find a good spawn point
			FindSpawnLocation();
		}
	}),
	FMath::RandRange(0.05f,0.2f),
	false);
}

void AGameManager::DecreaseEnemyCount()
{
	--CurrentEnemyCount;
	++EnemiesKilled;
}

void AGameManager::IncreaseEnemyCount()
{
	++CurrentEnemyCount;
}

void AGameManager::PrepareEliteSpawn()
{
	// TODO : EQS 
}

void AGameManager::CreateXPTable()
{
	for(int32 level = 0;level<=MaxLevel;++level)
	{
		int xp = FMath::TruncToInt((level * 0.7f + 1.0f)*50.0f) * level;

		XPArray.Add(xp);
	}
	NeededXP = XPToNextLevel();
}

int32 AGameManager::XPToNextLevel()
{
	return XPArray[Level + 1] - XPArray[Level];
}

void AGameManager::AddXp(int32 XP)
{
	if(Level>=MaxLevel)
		return;

	CurrentXP = CurrentXP+XP;

	UpdateCharactersXP(CurrentXP/NeededXP,Level);

	if(CurrentXP>=NeededXP)
	{
		Level = Level+1;

		CurrentXP = CurrentXP - NeededXP;

		NeededXP = XPToNextLevel();

		UpdateCharactersXP(CurrentXP/NeededXP,Level);

		PrepareLevelUp();
	}
}

void AGameManager::PrepareLevelUp()
{
	SavePlayerData();

	for(AGameplay_PlayerController* PC : PlayerControllerArray)
	{
		if(PC->GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			if(IInterface_CharacterManager::Execute_IsAlive(PC->GetPawn()))
			{
				if(PC->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
					IInterface_ControllerManager::Execute_OnLevelUp(PC);
				else
				{
					RS_LOG_ERROR(TEXT("Controller가 ControllerManager인터페이스를 상속받지 않았습니다."))
				}
			}
		}
		else
		{
			RS_LOG_ERROR(TEXT("Controller의 Pawn이 CharacterManager인터페이스를 상속받지 않았습니다."))
		}
	}

	for(AGameplay_PlayerController* PC : PlayerControllerArray)
	{
		IInterface_CharacterManager::Execute_Pause(PC->GetPawn(),true,false);
	}
}

void AGameManager::GameSetup()
{
	// Xp is managed in game manager to ensure Xp is synced between all players
	CreateXPTable();

	// Find All Players
	BuildPlayerArray();

	// If you need further references, set them here
	SetReferences();

	// Start Game Timer
	StartTimer();
}

void AGameManager::SpawnSetup()
{
	PrepareWaveElites();

	PrepareWaveEnemies();

	GetWorldTimerManager().SetTimer(SpawnTimerReference,FTimerDelegate::CreateLambda([&]()
	{
		// SpawnWave();
	}),
	SpawnInterval,
	true);

	GetWorldTimerManager().SetTimer(PhaseIndexTimer,FTimerDelegate::CreateLambda([&]()
	{
		IncreaseWaveIndex();
	}),
	60.0f,
	true);
}

void AGameManager::PrepareWaveEnemies()
{
	// first - Create enough items in array for each minute of game
	for(int time=0;time<=MaxGameTime;++time)
	{
		EliteSpawns.Add(FEnemySpawns());
	}

	// then populate based on data table
	TArray<FName> RowNames = EnemySpawnDT->GetRowNames();

	for(FName RowName : RowNames)
	{
		FEnemySpawnType* EnemySpawnType = EnemySpawnDT->FindRow<FEnemySpawnType>(RowName,TEXT("EnemySpawnDT"));

		EliteSpawns[EnemySpawnType->AllowedWave].Spawns.Add(*EnemySpawnType);
	}
}

void AGameManager::BuildPlayerArray()
{
	AGameStateBase* GameState = UGameplayStatics::GetGameState(GetWorld());
	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;

	for(APlayerState* PS : PlayerArray)
	{
		ABase_Character* Character = Cast<ABase_Character>(PS->GetPawn());
		PlayerCharacterArray.Add(Character);

		Character->GM_Interface = this;

		PlayerControllerArray.Add(Cast<AGameplay_PlayerController>(PS->GetPlayerController()));
	}
}

void AGameManager::SetReferences()
{
	// Empty
}

void AGameManager::StartTimer()
{
	UpdateTimer();

	GetWorldTimerManager().SetTimer(ClockReference,FTimerDelegate::CreateLambda([&]()
	{
		UpdateTimer();
	}),
	1.0f,
	true);
}

void AGameManager::PrepareWaveElites()
{
	// first - Create enough items in array for each minute of game
	for(int time=0;time<=MaxGameTime;++time)
	{
		EliteSpawns.Add(FEnemySpawns());
	}

	// then populate based on data table
	TArray<FName> RowNames = EnemyEliteSpawnDT->GetRowNames();

	for(FName RowName : RowNames)
	{
		FEnemySpawnType* EnemySpawnType = EnemyEliteSpawnDT->FindRow<FEnemySpawnType>(RowName,TEXT("EnemyEliteSpawnDT"));

		EliteSpawns[EnemySpawnType->AllowedWave].Spawns.Add(*EnemySpawnType);
	}
}

void AGameManager::SpawnEnemy(FEnvQueryInstance Instance, EEnvQueryStatus::Type Status)
{
	if(PreparedEnemies.IsEmpty())
	{
		RS_LOG_ERROR(TEXT("ERROR: Enemy data table not setup correctly!"))
		return;
	}
	// EQS 공부하고 할것.
}

void AGameManager::FindSpawnLocation()
{
	int32 randIndex = FMath::RandRange(0,PlayerCharacterArray.Num());

	ABase_Character* RandPlayer = PlayerCharacterArray[randIndex];
	if(!IsValid(RandPlayer))
	{
		RS_LOG_ERROR(TEXT("Random Player Character가 유효하지 않습니다."))
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* EQSBlueprintWrapper =  UEnvQueryManager::RunEQSQuery(GetWorld(),EQS_FindSpawnPoint,RandPlayer,EEnvQueryRunMode::AllMatching,nullptr);

	// EQS 찾아보고 오자.
	// EQSBlueprintWrapper->GetOnQueryFinishedEvent().Add(this,&AGameManager::SpawnEnemy);
}

void AGameManager::DetemineGameStatus()
{
	bool bFoundAlive = false;
	for(ABase_Character* Character : PlayerCharacterArray)
	{
		if(Character->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			if(IInterface_CharacterManager::Execute_IsAlive(Character))
			{
				bFoundAlive = true;
				break;
			}
		}
	}
	if(!bFoundAlive)
		EndGame(false);
}


void AGameManager::UpdateTimer()
{
	++Time;

	while(Time>59)
	{
		Time = Time-60;

		++Minutes;
	}

	FString second = Time<10?FString::Printf(TEXT("0%d"),Time):FString::Printf(TEXT("%d"),Time);
	
	GameTime = FText::FromString(FString::Printf(TEXT("%d:%s"),Minutes,*second));

	if(Minutes>=MaxGameTime)
	{
		EndGame(true);
	}
}

void AGameManager::ProcessEndGame()
{
	// Clear Timers
	GetWorldTimerManager().ClearTimer(SpawnTimerReference);

	GetWorldTimerManager().ClearTimer(PhaseIndexTimer);

	GetWorldTimerManager().ClearTimer(ClockReference);

	// End Player Timers and actions, then save
	for(ABase_Character* Character : PlayerCharacterArray)
	{
		if(Character->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			UAbilitiesComponent* AbilitiesComponent = IInterface_CharacterManager::Execute_GetAbilityComponent(Character);

			AbilitiesComponent->InvalidateTimers();

			Character->GetController()->SetIgnoreMoveInput(true);

			APlayerState* PS = Character->GetController()->PlayerState;

			if(PS->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
			{
				IInterface_PlayerState::Execute_CallGameSave(PS);
			}
		}
	}
}

void AGameManager::IncreaseWaveIndex()
{
	++WaveIndex;

	PrepareEliteSpawn();
}

void AGameManager::SpawnElite(FEnvQueryInstance Instance, EEnvQueryStatus::Type Status)
{
}

void AGameManager::UpdateEnemyWave()
{
	if(EnemySpawns[WaveIndex].Spawns.IsEmpty())
	{
		return;
	}

	PreparedEnemies = EnemySpawns[WaveIndex].Spawns;
}

void AGameManager::OnRep_GameTime()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if(Controller->GetClass()->ImplementsInterface(UInterface_ControllerManager::StaticClass()))
	{
		IInterface_ControllerManager::Execute_UpdateTime(Controller,GameTime);
	}
	else
	{
		RS_LOG_ERROR(TEXT("Controller가 ControllerManager 인터페이스를 상속받지 않았습니다."))
	}
}

void AGameManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameManager,GameTime);
}

