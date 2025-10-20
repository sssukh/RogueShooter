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
#include "UI/UW_ChestMaster.h"
#include "UI/UW_GameMenu.h"
#include "UI/UW_LevelUpMaster.h"
#include "UI/UW_PlayerHud.h"
#include "UI/UW_MatchResults.h"
#include "Utility/RSLog.h"

AGameplay_PlayerController::AGameplay_PlayerController()
{
	// Object와 Class 초기 설정 
	static ConstructorHelpers::FObjectFinder<UTexture2D> CoinIconFinder(*AssetPath::Texture::TwoCoins);
	if(CoinIconFinder.Succeeded())
		CoinIcon = CoinIconFinder.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> PotionIconFinder(*AssetPath::Texture::HealthPotion);
	if(PotionIconFinder.Succeeded())
		HealthPotionIcon = PotionIconFinder.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> WinSoundFinder(*AssetPath::Sound::WinSound);
	if(WinSoundFinder.Succeeded())
		WinSound = WinSoundFinder.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> LoseSoundFinder(*AssetPath::Sound::LoseSound);
	if(LoseSoundFinder.Succeeded())
		LoseSound = LoseSoundFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ActiveDTFinder(*AssetPath::DataTable::DT_ActiveAbilities);
	if(ActiveDTFinder.Succeeded())
		DT_ActiveAbilities = ActiveDTFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> PassiveDTFinder(*AssetPath::DataTable::DT_PassiveAbilities);
	if(PassiveDTFinder.Succeeded())
		DT_PassiveAbilities = PassiveDTFinder.Object;

	static ConstructorHelpers::FClassFinder<UUW_PlayerHud> PlayerHudClassFinder(*AssetPath::Blueprint::WBP_PlayerHud_C);
	if(PlayerHudClassFinder.Succeeded())
		PlayerHudClass = PlayerHudClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUW_LevelUpMaster> LevelUpMasterClassFinder(*AssetPath::Blueprint::WBP_LevelUpMaster_C);
	if(LevelUpMasterClassFinder.Succeeded())
		LevelUpMasterClass = LevelUpMasterClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUW_MatchResults> MatchResultClassFinder(*AssetPath::Blueprint::WBP_MatchResults_C);
	if(MatchResultClassFinder.Succeeded())
		MatchResultClass = MatchResultClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUW_ChestMaster> ChestMasterClassFinder(*AssetPath::Blueprint::WBP_ChestMaster_C);
	if(ChestMasterClassFinder.Succeeded())
		ChestMasterClass = ChestMasterClassFinder.Class;
}

void AGameplay_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	
	
	SetupPlayer();
}

void AGameplay_PlayerController::OC_CreateLevelupUI_Implementation()
{
	PreparingUI = true;

	LevelUpHudUp = true;

	SetShowMouseCursor(true);

	PrepareLevelUp();
}

void AGameplay_PlayerController::OC_CreateChestUI_Implementation()
{
	PreparingUI = true;

	ChestHudUp = true;

	SetShowMouseCursor(true);

	PrepareChest();
}

void AGameplay_PlayerController::OnPauseMenuCalled()
{
	Super::OnPauseMenuCalled();

	SetShowMouseCursor(true);

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(GameMenu->TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeUIOnly);
}

void AGameplay_PlayerController::OnPauseMenuClosed()
{
	Super::OnPauseMenuClosed();

	SetShowMouseCursor(false);

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}

void AGameplay_PlayerController::UpdateHudHotbar_Implementation(TMap<EActiveAbilities, int32>& Active,
                                                                TMap<EPassiveAbilities, int32>& Passive)
{
	IInterface_ControllerManager::UpdateHudHotbar_Implementation(Active, Passive);

	TArray<FAABIntPair> ActiveArr;
	for(const auto& Elem : Active)
	{
		ActiveArr.Add(FAABIntPair{Elem.Key,Elem.Value});
	}

	TArray<FPABIntPair> PassiveArr;
	for(const auto& Elem : Passive)
	{
		PassiveArr.Add(FPABIntPair{Elem.Key,Elem.Value});
	}
	
	OC_UpdateHudHotbar(ActiveArr,PassiveArr);
}

void AGameplay_PlayerController::UpdateLevelBar_Implementation(float Percent, int32 Level)
{
	// IInterface_ControllerManager::UpdateLevelBar_Implementation(Percent, Level);
	OC_UpdateCharUI(Percent,Level);
}

void AGameplay_PlayerController::UpdateTime_Implementation(FText& Time)
{
	IInterface_ControllerManager::UpdateTime_Implementation(Time);
	OC_UpdateTimer(Time);
}

void AGameplay_PlayerController::UpdateGold_Implementation(int32 Amount)
{
	IInterface_ControllerManager::UpdateGold_Implementation(Amount);
	OC_UpdateGoldInUI(Amount);
}

void AGameplay_PlayerController::OnMatchEnd_Implementation(bool Victory, int32 EnemiesKilled)
{
	IInterface_ControllerManager::OnMatchEnd_Implementation(Victory, EnemiesKilled);
	OC_EndMatch(Victory,EnemiesKilled);
}

void AGameplay_PlayerController::OnLevelUp_Implementation()
{
	IInterface_ControllerManager::OnLevelUp_Implementation();
	OC_CreateLevelupUI();
}

void AGameplay_PlayerController::OnChestFound_Implementation()
{
	IInterface_ControllerManager::OnChestFound_Implementation();
	OC_CreateChestUI();
}

void AGameplay_PlayerController::OC_UpdateHudHotbar_Implementation(const TArray<FAABIntPair>& Active,
	const TArray<FPABIntPair>& Passive)
{
	TMap<EActiveAbilities,int32> ActiveMap;
	for(FAABIntPair AABInt : Active)
	{
		ActiveMap.Add(AABInt.AAbility,AABInt.Value);
	}

	TMap<EPassiveAbilities,int32> PassiveMap;
	for(FPABIntPair PABInt : Passive)
	{
		PassiveMap.Add(PABInt.PAbility,PABInt.Value);
	}
	
	PlayerHud->BuildHotbar(ActiveMap,PassiveMap);
}


void AGameplay_PlayerController::OC_UpdateCharUI_Implementation(float Percent, int32 Level)
{
	UpdateCharacterUI(Percent,Level);
}


void AGameplay_PlayerController::OC_UpdateTimer_Implementation(const FText& Time)
{
	PlayerHud->UpdateTime(Time);
}


void AGameplay_PlayerController::OC_UpdateGoldInUI_Implementation(int32 Amount)
{
	PlayerHud->UpdateGold(Amount);
}

void AGameplay_PlayerController::OC_EndMatch_Implementation(bool Victory, int32 EnemiesKilled)
{
	ShowEndMatchScreen(Victory,EnemiesKilled);
}

void AGameplay_PlayerController::SetupPlayer()
{
	if(!IsLocalPlayerController())
		return;

	CleanUpUI();

	CreateGameplayUI();

	SetReference();

	AbilityComponent->SetStartingAbility();

	UpdateHotbar();
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

	PlayerHud->TextBlock_Level->SetText(FText::FromString(FString::Printf(TEXT("Level %d"),Level)));
}

void AGameplay_PlayerController::UpdateHotbar()
{
	Execute_UpdateHudHotbar(this,AbilityComponent->ActiveAbilitiesMap,AbilityComponent->PassiveAbilitiesMap);
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
				NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(EvoAAbility)).Append(FString::Printf(TEXT("%d"),-1));
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
				EActiveAbilities ActiveAbility = AvailableActiveAbilities[FMath::RandRange(0,AvailableActiveAbilities.Num()-1)];

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
					
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"), true);
					FString NameString;
					if(EnumPtr)
					{
						NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(ActiveAbility));
					}
					if(Name.ToString() == NameString.Append(FString::Printf(TEXT("%d"),level)))
					{
						FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(Name,TEXT("DT_ActiveAbilities"));

						if(AbilityLevelUp)
						{
							// 왜 aability랑 pability에 직접 값들을 넣어놨을까?
							LevelUpUI->AddSelection(FText::FromString(NameString),level,AbilityLevelUp->LevelUpText,
								UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),ActiveAbility),
								ActiveAbility,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Active);

							break;
						}
					}
				}
			}
			else
			{
				EPassiveAbilities PassiveAbility = AvailablePassiveAbilities[FMath::RandRange(0,AvailablePassiveAbilities.Num()-1)];

				AvailablePassiveAbilities.Remove(PassiveAbility);

				CanAddPassiveAbility = !AvailablePassiveAbilities.IsEmpty();

				TArray<FName> PAbilityNames = DT_PassiveAbilities->GetRowNames();

				for(const FName& Name : PAbilityNames)
				{
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EPassiveAbilities"), true);
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
	Ability = EActiveAbilities::Hammer;
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
			if(PassiveArray.Contains(*PAbility))
			{
				Ability = AAbility;
				return true;
			}
		}
	}

	return false;
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

		SetShowMouseCursor(false);

		FInputModeGameOnly InputGameOnly;
		
		SetInputMode(InputGameOnly);
	}

	// Level Up Ability
	AssignAbility(Type,PAbilities,AAbilities);

	
	// Refresh after ability is modified
	UpdateHotbar();

	AbilityComponent->RefreshAbilities();
}

void AGameplay_PlayerController::PrepareChest()
{
	// Check if chest UI has been created
	// if so refresh UI and build new cards to show abilities
	// if not create chest UI and save reference
	if(ChestUI != nullptr)
	{
		ChestUI->ResetUI();
	}
	else
	{
		if(UUW_ChestMaster* ChestMaster = CreateWidget<UUW_ChestMaster>(this,ChestMasterClass))
		{
			ChestUI = ChestMaster;

			ChestMaster->OnClose.AddDynamic(this,&AGameplay_PlayerController::CloseChestUI);
		}	
	}

	ChestUI->AddToViewport(1);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ChestUI->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	// Determine how many abilities to add and gold to give
	
	int32 ChestCount = DetermineChestCount();

	ChestUI->Gold = ChestUI->Gold + ChestCount * 25;

	for(int i=0;i<ChestCount;++i)
	{
		BuildAndProcessChest(i);
	}
	if(!PlayerState->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("PlayerState가 PlayerStateInterface를 상속받지 않았습니다."))
		return;
	}
	IInterface_PlayerState::Execute_OnGoldPickUp(PlayerState,ChestUI->Gold);

}

int32 AGameplay_PlayerController::DetermineChestCount()
{
	int32 Roll = FMath::RandRange(1,100);

	if(Roll<70)
		return 0;
	if(Roll<90)
		return 1;
	return 2;
}

void AGameplay_PlayerController::BuildAndProcessChest(int32 index)
{
	int32 Index = index;

	TMap<EActiveAbilities,int32> ActiveAbilityMap = AbilityComponent->ActiveAbilitiesMap;
	TMap<EPassiveAbilities,int32> PassiveAbilityMap = AbilityComponent->PassiveAbilitiesMap;
	int32 AbilityMaxLevel = AbilityComponent->MaxAbilityLevel;

	bool bCanAddActiveAbility = true;
	bool bCanAddPassiveAbility = true;
	
	// Get Abilities that are not maxed out
	TArray<EActiveAbilities> AvailableActiveAbilities = Chest_BuildActiveList(ActiveAbilityMap,AbilityMaxLevel);

	if(AvailableActiveAbilities.IsEmpty())
		bCanAddActiveAbility = false;

	TArray<EPassiveAbilities> AvailablePassiveAbilities = Chest_BuildPassiveList(PassiveAbilityMap,AbilityMaxLevel);

	if(AvailablePassiveAbilities.IsEmpty())
		bCanAddPassiveAbility = false;

	if(bCanAddActiveAbility || bCanAddPassiveAbility)
	{
		if(bCanAddActiveAbility)
			{
				// Remove from local array so we do not have duplicate
				EActiveAbilities ActiveAbility = AvailableActiveAbilities[FMath::RandRange(0,AvailableActiveAbilities.Num()-1)];

				AvailableActiveAbilities.Remove(ActiveAbility);

				bCanAddActiveAbility = !AvailableActiveAbilities.IsEmpty();

				TArray<FName> AAbilityNames = DT_ActiveAbilities->GetRowNames();

				for(const FName& Name : AAbilityNames)
				{
					int32 level = 0;
					int32* findLevel = ActiveAbilityMap.Find(ActiveAbility);

					if(findLevel)
						level = *findLevel;

					++level;
					
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EActiveAbilities"), true);
					FString NameString;
					if(EnumPtr)
					{
						NameString = EnumPtr->GetNameStringByValue(static_cast<int64>(ActiveAbility));
					}
					if(Name.ToString() == NameString.Append(FString::Printf(TEXT("%d"),level)))
					{
						FAbilityLevelUp* AbilityLevelUp = DT_ActiveAbilities->FindRow<FAbilityLevelUp>(Name,TEXT("DT_ActiveAbilities"));

						if(AbilityLevelUp)
						{
							ChestUI->AddSelection(FText::FromString(NameString),level,AbilityLevelUp->LevelUpText,
								UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),ActiveAbility),
								EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Active);
							AssignAbility(EAbilityType::Active,EPassiveAbilities::Ability_Bonus_Damage,ActiveAbility);
							break;
						}
					}
				}
			}
			else
			{
				EPassiveAbilities PassiveAbility = AvailablePassiveAbilities[FMath::RandRange(0,AvailablePassiveAbilities.Num()-1)];

				AvailablePassiveAbilities.Remove(PassiveAbility);

				bCanAddPassiveAbility = !AvailablePassiveAbilities.IsEmpty();

				TArray<FName> PAbilityNames = DT_PassiveAbilities->GetRowNames();

				for(const FName& Name : PAbilityNames)
				{
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE,TEXT("EPassiveAbilities"), true);
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
							int32* findLevel = PassiveAbilityMap.Find(PassiveAbility);
							if(findLevel)
								level = *findLevel;

							++level;

							ChestUI->AddSelection(FText::FromString(NameString),level,AbilityLevelUp->LevelUpText,
								UFunctionLibrary_Helper::FindPassiveIcon(GetWorld(),PassiveAbility),EActiveAbilities::Hammer,PassiveAbility,EAbilityType::Passive);
							AssignAbility(EAbilityType::Passive,PassiveAbility,EActiveAbilities::Hammer);
							break;
						}
					}
				}
			}
	}
	else
	{
		// If no abilities can be added - give bonus gold
		ChestUI->AddSelection(FText::FromString(TEXT("Bonus")),0,FText::FromString(TEXT("50 Gold")),CoinIcon,
			EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage,EAbilityType::Active);
		AssignAbility(EAbilityType::Gold,EPassiveAbilities::Ability_Bonus_Damage,EActiveAbilities::Hammer);
	}
}

TArray<EActiveAbilities> AGameplay_PlayerController::Chest_BuildActiveList(TMap<EActiveAbilities, int32> ActiveMap,
	int32 MaxLevel)
{
	TArray<EActiveAbilities> Result;
	TMap<EActiveAbilities,int32> TempActiveMap = ActiveMap;
	int32 MaxAbilityLevel = MaxLevel;

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

TArray<EPassiveAbilities> AGameplay_PlayerController::Chest_BuildPassiveList(TMap<EPassiveAbilities, int32> PassiveMap,
	int32 MaxLevel)
{
	TArray<EPassiveAbilities> Result;
	TMap<EPassiveAbilities,int32> TempActiveMap = PassiveMap;
	int32 MaxAbilityLevel = MaxLevel;

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

void AGameplay_PlayerController::CloseChestUI()
{
	ChestHudUp = false;

	if(!PlayerState->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("PlayserState가 PlayerState인터페이스를 상속받지 않았습니다."))
		return;
	}

	IInterface_PlayerState::Execute_CallGameSave(PlayerState);

	// if level up UI still up - skip reset
	if(!LevelUpHudUp)
	{
		// Reset UI
		PreparingUI = false;

		if(!GetPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			RS_LOG_ERROR(TEXT("Pawn이 Charactermanager인터페이스를 상속받지 않았습니다."))
			return;
		}
		IInterface_CharacterManager::Execute_Pause(GetPawn(),false,false);

		bShowMouseCursor = false;

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
	}

	// Update UI and abilities when abiltiy is modified
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
