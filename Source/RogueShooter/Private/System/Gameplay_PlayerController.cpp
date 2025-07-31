// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Gameplay_PlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/AbilitiesComponent.h"
#include "Interface/Interface_CharacterManager.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_LevelUpMaster.h"
#include "UI/UW_PlayerHud.h"

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
		if(0.5f <= FMath::RandRange(0.0f,1.0f))
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
