// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueShooter/Public/System/Base_GameMode.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Character/Base_Character.h"
#include "Components/MonsterPoolComponent.h"
#include "Components/WaveManagerComponent.h"
#include "Enemies/Base_Enemy.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Interface/Interface_CharacterManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_ControllerManager.h"
#include "Interface/Interface_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "System/Base_GameState.h"
#include "System/Gameplay_PlayerController.h"
#include "Utility/RSLog.h"

ABase_GameMode::ABase_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WaveManagerComponent = CreateDefaultSubobject<UWaveManagerComponent>(TEXT("WaveManagerComponent"));
	
	WaveManagerComponent->bEditableWhenInherited = true;
	
	MonsterPoolComponent = CreateDefaultSubobject<UMonsterPoolComponent>(TEXT("MonsterPoolComponent"));
	
	MonsterPoolComponent->bEditableWhenInherited =  true;
}

void ABase_GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle Delay;
	GetWorldTimerManager().SetTimer(Delay, FTimerDelegate::CreateLambda([&]()
	{
		GameSetup();
		// SpawnSetup();
		WaveManagerComponent->StartWave(1);
	}), 2.0f, false);
}

void ABase_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_GameMode::GameSetup()
{
	// 모든 플레이어 저장 
	BuildPlayerArray();
	
	UpdateTimer();
}

void ABase_GameMode::SpawnSetup()
{
	// Spawn 로직 추가하기 
	SpawnWave();
}

void ABase_GameMode::BuildPlayerArray()
{
	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;

	for(APlayerState* PS : PlayerArray)
	{
		ABase_Character* Character = Cast<ABase_Character>(PS->GetPawn());
		CachedPlayerCharacterArray.Add(Character);


		PlayerControllerArray.Add(Cast<AGameplay_PlayerController>(PS->GetPlayerController()));
	}
}

void ABase_GameMode::SpawnWave()
{
	GetWorldTimerManager().SetTimer(SpawnTimerReference,this,&ABase_GameMode::FindSpawnLocation,1.0f,true);
}


void ABase_GameMode::FindSpawnLocation()
{
	// GameState의 PlayerArray를 활용하여 랜덤 플레이어 찾기
	if (GameState && GameState->PlayerArray.Num() > 0)
	{
		int32 RandIndex = FMath::RandRange(0, GameState->PlayerArray.Num() - 1);
		APawn* PlayerPawn = GameState->PlayerArray[RandIndex]->GetPawn();
        
		if (PlayerPawn)
		{
			// EQS 실행 (기존 코드와 동일, this는 이제 GameMode)
			UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
				GetWorld(), EQS_FindSpawnPoint, PlayerPawn, EEnvQueryRunMode::AllMatching, nullptr);

			if (QueryInstance)
			{
				// 콜백 함수 바인딩
				QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ABase_GameMode::SpawnEnemy);
			}
		}
	}
}

void ABase_GameMode::SpawnEnemy(class UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status)
{
	TArray<FVector> ResultLocations;
	
	
	if(!Instance->GetQueryResultsAsLocations(ResultLocations))
	{
		FindSpawnLocation();

		RS_LOG_ERROR(TEXT("ERROR : Did Not Find Spawn Location"))
		RS_LOG_SCREEN_ERROR(TEXT("ERROR : Did Not Find Spawn Location"))
		return;
	}
	
	FVector Location = ResultLocations[FMath::RandRange(0,ResultLocations.Num()-1)] + FVector(0.0f,0.0f,25.0f);

	FTransform Transform = FTransform::Identity;
	Transform.SetTranslation(Location);
	
	if(ABase_Enemy* Enemy = GetWorld()->SpawnActorDeferred<ABase_Enemy>(EnemyClass,Transform))
	{
		Enemy->PlayerArray = CachedPlayerCharacterArray;

		Enemy->CharLevel = 1;

		Enemy->FinishSpawning(Transform);

		if(Enemy->GetController()==nullptr)
		{
			RS_LOG_ERROR(TEXT("%s has not possessed by controller"),*Enemy->GetName())
		}
		
		// Keep Track of Total Enemies
		Enemy->OnDeath.AddDynamic(this,&ABase_GameMode::IncreaseEnemyKill);

	}
}

void ABase_GameMode::SpawnElite(class UEnvQueryInstanceBlueprintWrapper* Instance, EEnvQueryStatus::Type Status)
{
}



void ABase_GameMode::EndGame(bool Victory)
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

void ABase_GameMode::UpdateTimer()
{
	Time++;
	while (Time > 59)
	{
		Time -= 60;
		Minutes++;
	}

	// ★ 중요: 데이터 갱신은 GameState에게 시킴
	if (ABase_GameState* MyGameState = GetGameState<ABase_GameState>())
	{
		MyGameState->UpdateGameTime(Minutes, Time);
	}
}

void ABase_GameMode::IncreaseWaveIndex()
{
}

void ABase_GameMode::DetermineGameStatus()
{
	bool bFoundAlive = false;
	for(ABase_Character* Character : CachedPlayerCharacterArray)
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

void ABase_GameMode::OnPlayerDeath()
{
}

void ABase_GameMode::ProcessEndGame()
{
	// Clear Timers
	GetWorldTimerManager().ClearTimer(SpawnTimerReference);

	GetWorldTimerManager().ClearTimer(PhaseIndexTimer);

	GetWorldTimerManager().ClearTimer(ClockReference);

	// End Player Timers and actions, then save
	for(ABase_Character* Character : CachedPlayerCharacterArray)
	{
		if(Character->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			Character->GetController()->SetIgnoreMoveInput(true);

			APlayerState* PS = Character->GetController()->PlayerState;

			if(PS->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
			{
				IInterface_PlayerState::Execute_CallGameSave(PS);
			}
		}
	}
}

void ABase_GameMode::IncreaseEnemyKill()
{
	if (ABase_GameState* GS = Cast<ABase_GameState>(GameState))
	{
		GS->EnemiesKilled++;
	}
}

void ABase_GameMode::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void ABase_GameMode::ResetPauseCount()
{
	PauseCount = 0;
}

bool ABase_GameMode::CheckPlayersForPause()
{
	if(GameIsPaused)
	{
		int32 playersCount = GetAlivePlayers();

		if(++PauseCount >= playersCount)
		{
			PauseCount = 0;

			return true;
		}

	}
	return false;
}

int32 ABase_GameMode::GetAlivePlayers()
{
	AGameStateBase* GameStateBase = GetGameState<AGameStateBase>();

	int32 alivePlayers = 0;

	// playerstate 순회하며 살아있는 플레이어의 수 얻기 
	for(const APlayerState* ps : GameStateBase->PlayerArray)
	{
		if(IInterface_CharacterManager::Execute_IsAlive(ps->GetPawn()))
		{
			++alivePlayers;
		}
	}
	return alivePlayers;
}

// TODO command
void ABase_GameMode::ServerTravel_Lobby()
{
	FString cmd = "/Game/RogueShooter/Maps/M_Lobby";
	
	GetWorld()->ServerTravel(cmd);
	
}

// TODO command
void ABase_GameMode::ServerTravel_GamePlay(FName Map)
{
	FString cmd = FString::Printf(TEXT("/Game/RogueShooter/Maps/%s"),*Map.ToString());

	GetWorld()->ServerTravel(cmd);
}

void ABase_GameMode::SetEnemyPlayerArray(ABase_Enemy* InEnemy)
{
	InEnemy->PlayerArray = CachedPlayerCharacterArray;
}
