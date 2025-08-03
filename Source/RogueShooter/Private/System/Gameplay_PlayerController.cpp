// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Gameplay_PlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/AbilitiesComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_CharacterManager.h"
#include "Interface/Interface_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_LevelUpMaster.h"
#include "UI/UW_PlayerHud.h"
#include "UI/UW_MatchResults.h"
#include "Utility/RSLog.h"

AGameplay_PlayerController::AGameplay_PlayerController()
{
}



void AGameplay_PlayerController::SetupPlayer()
{
	if(!IsLocalPlayerController())
		return;

	CleanUpUI();

	CreateGameplayUI();

	SetReference();
}

void AGameplay_PlayerController::CreateGameplayUI()
{
	if(PlayerHud == nullptr)
	{
		PlayerHud = CreateWidget<UUW_PlayerHud>(this,PlayerHudClass);
	}

	if(!PlayerHud->IsInViewport())
		PlayerHud->AddToViewport();
}

void AGameplay_PlayerController::SetReference()
{
	if(GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		AbilityComponent = IInterface_CharacterManager::Execute_GetAbilityComponent(GetPawn());
	}
}

void AGameplay_PlayerController::UpdateCharacterUI(float Percent, int32 Level)
{
	PlayerHud->ProgressBar_XP->SetPercent(Percent);

	PlayerHud->TextBlock_Level->SetText(FText::FromString(TEXT("Level %d",Level)));
}

void AGameplay_PlayerController::UpdateHotbar()
{
	UpdateHudHotbar(AbilityComponent->ActiveAbilitiesMap,AbilityComponent->PassiveAbilitiesMap);
}

void AGameplay_PlayerController::ShowEndMatchScreen(bool Victory, int32 EnemiesKilled)
{
	CleanUpUI();

	if(UUW_MatchResults* MatchResultWidget = CreateWidget<UUW_MatchResults>(this,MatchResultClass))
	{
		MatchResultWidget->Victory = Victory;
		MatchResultWidget->Kills = EnemiesKilled;

		MatchResultWidget->AddToViewport();

		if(Victory)
		{
			UGameplayStatics::PlaySound2D(GetWorld(),WinSound);
		}
		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(),LoseSound);
		}
	}
}

void AGameplay_PlayerController::PrepareLevelUp()
{
	// Build the level up UI based on current player data
	if(LevelUpUI == nullptr)
	{
		LevelUpUI = CreateWidget<UUW_LevelUpMaster>(this,LevelUpMasterClass);

		ExecuteLevelUp();

		LevelUpUI->OnReady.AddDynamic(this,&AGameplay_PlayerController::ExecuteLevelUp);

		LevelUpUI->OnClose.AddDynamic(this, &AGameplay_PlayerController::ProcessLevelUp);
	}
	// If widget already exists, just reset it
	else
	{
		LevelUpUI->ResetUI();
	}

	LevelUpUI->AddToViewport(1);

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(LevelUpUI->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeUIOnly);
}

void AGameplay_PlayerController::ExecuteLevelUp()
{
	// Prepare local variables
	TMap<EActiveAbilities,int32> AAbilityMap = AbilityComponent->ActiveAbilitiesMap;
	TMap<EPassiveAbilities,int32> PAbilityMap = AbilityComponent->PassiveAbilitiesMap;
	int32 AbilityMaxLevel = AbilityComponent->MaxAbilityLevel;

	
	
	// Check if an evolution for ability is ready
	EActiveAbilities EvoAAbility;
	bool EvoReady = CheckIfEvoReady(EvoAAbility);

	// Set amount of cards to show
	int32 MaxCount =4;

	int32 CardCount =0 ;

	// Add Evolution : If Ready
	if(EvoReady)
	{
		++CardCount;

		TArray<FName>ActiveAbilityNames = DT_ActiveAbilities->GetRowNames();

		for(const FName& AAbilityName : ActiveAbilityNames)
		{
			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"), true);
			FString NameString;
			if(EnumPtr)
			{
				NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(EvoAAbility)).Append(TEXT("%d",-1));
			}
			
			if(AAbilityName == FName(NameString))
			{
				FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(AAbilityName,TEXT("DT_ActiveAbilities"));
				if(AbilityLevelUp)
				{
					LevelUpUI->AddSelection(FText::FromString(NameString),-1,AbilityLevelUp->LevelUpText,
						UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),EvoAAbility),EvoAAbility,EPassiveAbilities::Ability_Bonus_Damage,
						EAbilityType::Evolution);
					break;
				}
			}
		}

	}

	// Get Abilities that are not maxed out
	TArray<EActiveAbilities> AvailableActiveAbilities = CheckActiveAbilities(AAbilityMap,AbilityMaxLevel);

	bool CanAddActiveAbility = true;
	if(AvailableActiveAbilities.IsEmpty())
		CanAddActiveAbility = false;

	TArray<EPassiveAbilities> AvailablePassiveAbilities = CheckPassiveAbilities(PAbilityMap,AbilityMaxLevel);

	bool CanAddPassiveAbility = true;
	if(AvailablePassiveAbilities.IsEmpty())
		CanAddPassiveAbility = false;

	// Run loop until we hit our desired cards or cannot add passive/active skill
	while(CardCount<MaxCount && (CanAddActiveAbility || CanAddPassiveAbility))
	{
		// Random bool to determine active or passive and failsafe incase one is not allowed
		++CardCount;

		// TODO : weight에 따라 active나 passive를 우선적으로 추가하는 순서를 설정하도록 해야겠다.
		// weight에 따라 Active를 우선적으로 추가할수도 Passive를 우선적으로 추가할수도 있다.
		// if(0.5f <= FMath::RandRange(0.0f,1.0f))
		{
			if(CanAddActiveAbility)
			{
				// Remove from local array so we do not have duplicate
				EActiveAbilities ActiveAbility = AvailableActiveAbilities[FMath::RandRange(0,AvailableActiveAbilities.Num())];

				AvailableActiveAbilities.Remove(ActiveAbility);

				CanAddActiveAbility = !AvailableActiveAbilities.IsEmpty();

				TArray<FName> AAbilityNames = DT_ActiveAbilities->GetRowNames();

				for(const FName& Name : AAbilityNames)
				{
					int32 level = 0;
					int32* findLevel = AAbilityMap.Find(ActiveAbility);

					if(findLevel)
						level = *findLevel;

					++level;

					// TODO : enum을 string으로 변환하는 함수를 헬퍼로 만들어두자.
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"), true);
					FString NameString;
					if(EnumPtr)
					{
						NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(ActiveAbility));
					}
					if(Name.ToString() == NameString.Append(TEXT("%d",level)))
					{
						FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(Name,TEXT("DT_ActiveAbilities"));

						if(AbilityLevelUp)
						{
							LevelUpUI->AddSelection(FText::FromString(NameString),level,AbilityLevelUp->LevelUpText,
								UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),ActiveAbility),
								EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Active);

							break;
						}
					}
				}
			}
			else
			{
				EPassiveAbilities PassiveAbility = AvailablePassiveAbilities[FMath::RandRange(0,AvailablePassiveAbilities.Num())];

				AvailablePassiveAbilities.Remove(PassiveAbility);

				CanAddPassiveAbility = !AvailablePassiveAbilities.IsEmpty();

				TArray<FName> PAbilityNames = DT_PassiveAbilities->GetRowNames();

				for(const FName& Name : PAbilityNames)
				{
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"), true);
					FString NameString;
					if(EnumPtr)
					{
						NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(PassiveAbility));
					}

					if(Name.ToString() == NameString)
					{
						FAbilityLevelUp* AbilityLevelUp = DT_PassiveAbilities->FindRow<FAbilityLevelUp>(Name,TEXT("DT_PassiveAbilities"));

						if(AbilityLevelUp)
						{
							int32 level = 0;
							int32* findLevel = PAbilityMap.Find(PassiveAbility);
							if(findLevel)
								level = *findLevel;

							++level;

							LevelUpUI->AddSelection(FText::FromString(NameString),level,AbilityLevelUp->LevelUpText,
								UFunctionLibrary_Helper::FindPassiveIcon(GetWorld(),PassiveAbility),EActiveAbilities::Hammer,PassiveAbility,EAbilityType::Passive);

							break;
						}
					}
				}
			}
		}
	}
	
	// If no abilities can be added - give option for health or gold
	if(CardCount == 0)
	{
		LevelUpUI->AddSelection(FText::FromString(TEXT("Health Potion")),0,FText::FromString(TEXT("Recover Health to Full")),
			HealthPotionIcon,EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Health);

		LevelUpUI->AddSelection(FText::FromString(TEXT("Gold Pile")),0,FText::FromString(TEXT("Add 50 Gold")),
			CoinIcon,EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Gold);
	}
}

bool AGameplay_PlayerController::CheckIfEvoReady(EActiveAbilities& Ability)
{
	TArray<EPassiveAbilities> PassiveArray = AbilityComponent->EvolutionPassiveArray;

	TMap<EActiveAbilities,EPassiveAbilities> EvoMap = AbilityComponent->EvolutionMap;

	if(EvoMap.IsEmpty())
	{
		Ability = EActiveAbilities::Hammer;
		return false;
	}

	TArray<EActiveAbilities> ActiveAbilities;
	EvoMap.GetKeys(ActiveAbilities);

	for(EActiveAbilities AAbility : ActiveAbilities)
	{
		if(EPassiveAbilities* PAbility = EvoMap.Find(AAbility))
		{
			if(PassiveArray.Contains(PAbility))
			{
				Ability = AAbility;
				return true;
			}
		}
	}

	
}

TArray<EActiveAbilities> AGameplay_PlayerController::CheckActiveAbilities(TMap<EActiveAbilities, int32> ActiveMap,
	int32 MaxLevel)
{
	TArray<EActiveAbilities> Result;
	
	int32 MaxAbilityLevel = MaxLevel;

	TMap<EActiveAbilities,int32> TempActiveMap = ActiveMap;

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"),true);
	int32 EnumNum = EnumPtr->NumEnums();
	// Check against max abount of active abilities
	if(TempActiveMap.Num()<EnumNum)
	{
		for(EActiveAbilities AAbility : TEnumRange<EActiveAbilities>())
		{
			Result.Add(AAbility);
		}
		
		// Remove Abilities that are maxed out
		TArray<EActiveAbilities> AAbilities;
		TempActiveMap.GetKeys(AAbilities);

		for(EActiveAbilities aability : AAbilities)
		{
			if(TempActiveMap[aability]>=MaxAbilityLevel)
			{
				Result.Remove(aability);
			}
		}
		// Return any abilities that are not yet learned or maxed out
		return Result;
	}
	else
	{
		// Add Abilities that are not maxed out
		TArray<EActiveAbilities> AAbilities;
		TempActiveMap.GetKeys(AAbilities);

		for(EActiveAbilities aability : AAbilities)
		{
			if(TempActiveMap[aability]<MaxAbilityLevel)
			{
				Result.Add(aability);
			}
		}

		// return any abilities that are not yet maxed out
		return Result;
	}
}

TArray<EPassiveAbilities> AGameplay_PlayerController::CheckPassiveAbilities(TMap<EPassiveAbilities, int32> PassiveMap,
	int32 MaxLevel)
{
	TArray<EPassiveAbilities> Result;
	
	int32 MaxAbilityLevel = MaxLevel;

	TMap<EPassiveAbilities,int32> TempActiveMap = PassiveMap;

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EPassiveAbilities"),true);
	int32 EnumNum = EnumPtr->NumEnums();
	
	// Check against max abount of active abilities
	if(TempActiveMap.Num()<EnumNum)
	{
		for(EPassiveAbilities PAbility : TEnumRange<EPassiveAbilities>())
		{
			Result.Add(PAbility);
		}
		
		// Remove Abilities that are maxed out
		TArray<EPassiveAbilities> PAbilities;
		TempActiveMap.GetKeys(PAbilities);

		for(EPassiveAbilities pability : PAbilities)
		{
			if(TempActiveMap[pability]>=MaxAbilityLevel)
			{
				Result.Remove(pability);
			}
		}
		// Return any abilities that are not yet learned or maxed out
		return Result;
	}
	else
	{
		// Add Abilities that are not maxed out
		TArray<EPassiveAbilities> PAbilities;
		TempActiveMap.GetKeys(PAbilities);

		for(EPassiveAbilities pability : PAbilities)
		{
			if(TempActiveMap[pability]<MaxAbilityLevel)
			{
				Result.Add(pability);
			}
		}

		// return any abilities that are not yet maxed out
		return Result;
	}
}

void AGameplay_PlayerController::ProcessLevelUp(EAbilityType Type, EActiveAbilities AAbilities,
                                                EPassiveAbilities PAbilities)
{
	LevelUpHudUp = false;

	// Reset UI & Unpause
	if(ChestHudUp)
	{
		PreparingUI = false;

		if(!GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			RS_LOG_ERROR(TEXT("Pawn이 IInterface_CharacterManager를 상속받지 않았습니다."))
			return;
		}
		
		IInterface_CharacterManager::Execute_Pause(GetPawn(),false,false);

		bShowMouseCursor =false;

		FInputModeGameOnly InputGameOnly;
		
		SetInputMode(InputGameOnly);
	}

	// Level Up Ability
	AssignAbility(Type,PAbilities,AAbilities);

	
	// Refresh after ability is modified
	UpdateHotbar();

	AbilityComponent->RefreshAbilities();
}

void AGameplay_PlayerController::ActivateEvolution(EActiveAbilities Evo)
{
	AbilityComponent->EvolutionMap.Remove(Evo);

	AbilityComponent->EvolutionTracker[(int32)Evo] = true;

	// Do Specific logic here that can't be handled in ability preparation
	if(Evo == EActiveAbilities::Frost_Bolt)
	{
		AbilityComponent->FBTimer = 0.2f;
	}
}

void AGameplay_PlayerController::AssignAbility(EAbilityType Type, EPassiveAbilities PAbility, EActiveAbilities AAbility)
{
	switch (Type)
	{
	case EAbilityType::Active:
		switch (AAbility)
		{
		case EActiveAbilities::Hammer:
			AbilityComponent->LevelUpHammer();
			break;
		case EActiveAbilities::Fireball:
			AbilityComponent->LevelUpFireball();
			break;
		case EActiveAbilities::Lightning:
			AbilityComponent->LevelUpLightning();
			break;
		case EActiveAbilities::Frost_Bolt:
			AbilityComponent->LevelUpFrostBolt();
			break;
	default :
		break;
		}
		break;
	case EAbilityType::Passive:
		switch (PAbility)
		{
		case EPassiveAbilities::Ability_Bonus_Damage:
			AbilityComponent->LevelUpAbilityDamageBonus(false);
			break;
		case EPassiveAbilities::Health_Bonus:
			AbilityComponent->LevelUpMaxHealth(false);
			break;
		case EPassiveAbilities::Speed_Bonus:
			AbilityComponent->LevelUpSpeedBonus(false);
			break;
		case EPassiveAbilities::Ability_Cooldown_Reduction:
			AbilityComponent->LevelUpTimerReduction(false);
			break;
		default:
			break;
		}
		break;
	case EAbilityType::Evolution:
		ActivateEvolution(AAbility);
		break;
	case EAbilityType::Gold:
		if(!GetPawn()->GetPlayerState()->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
		{
			RS_LOG_ERROR(TEXT("PlayerState가 PlayerState 인터페이스를 상속받지 않았습니다."))
			return;
		}
		IInterface_PlayerState::Execute_OnGoldPickUp(GetPawn()->GetPlayerState(),50);
		break;
	case EAbilityType::Health:
		if(!GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			RS_LOG_ERROR(TEXT("Pawn이 CharacterManager 인터페이스를 상속받지 않았습니다."))
			return;
		}
		IInterface_CharacterManager::Execute_RestoreHealth(GetPawn(),9999.0f);
		break;
	default:
		break;
	}
}
