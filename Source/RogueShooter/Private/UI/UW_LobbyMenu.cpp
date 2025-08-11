// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LobbyMenu.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Interface/Interface_CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "Saves/SG_Player.h"
#include "System/Base_GameInstance.h"
#include "System/Base_GameMode.h"
#include "UI/UW_CharacterSelectionItem.h"
#include "Utility/HelperFunctions.h"
#include "Utility/RSLog.h"

UUW_LobbyMenu::UUW_LobbyMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ACDataTableFinder(*AssetPath::DataTable::DT_AvailableCharacter);

	if(ACDataTableFinder.Succeeded())
		DT_AvailableCharacters = ACDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> AMDataTableFinder(*AssetPath::DataTable::DT_AvailableMaps);

	if(AMDataTableFinder.Succeeded())
		DT_AvailableMaps = AMDataTableFinder.Object;
	
}

void UUW_LobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Launch->OnClicked.AddDynamic(this,&UUW_LobbyMenu::OnButtonLaunchClicked);

	LoadData();

	HostCheck();

	BuildMapSelection();
}

void UUW_LobbyMenu::OnButtonLaunchClicked()
{
	UFunctionLibrary_Helper::CreateLoadingScreen(GetWorld(),
		FText::FromString(TEXT("Seamless travel  does not work in editor, must launch standalone or compiled game to travel between lobby and game play maps"))
		,LoadingScreenClass);

	UBase_GameInstance* GameInstance = Cast<UBase_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABase_GameMode* GameMode = Cast<ABase_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ServerTravel_GamePlay(GameInstance->CurrentMap);
}

void UUW_LobbyMenu::LoadData()
{
	GameSave = UFunctionLibrary_Helper::LoadPlayerData(GetWorld());

	UnlockedCharacters = GameSave->UnlockedCharacters;

	UpdateGold(GameSave->Gold);

	CreateCharacterList();
}

void UUW_LobbyMenu::UpdateGold(int32 Gold)
{
	TextBlock_Gold->SetText(FText::FromString(FString::Printf(TEXT("%d"),Gold)));
}

void UUW_LobbyMenu::HostCheck()
{
	if(!(GetWorld() && GetWorld()->GetNetMode() != NM_Client))
	{
		SizeBox_ServerButton->SetVisibility(ESlateVisibility::Collapsed);

		Border_MapSelection->SetVisibility(ESlateVisibility::Collapsed);

		TextBlock_ClientStatus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUW_LobbyMenu::CreateCharacterList()
{
	ScrollBox_Characters->ClearChildren();

	TArray<FName> RowNames = DT_AvailableCharacters->GetRowNames();

	TArray<FString> UnlockedCharacterStrings;

	for(FText Name : UnlockedCharacters)
	{
		UnlockedCharacterStrings.AddUnique(Name.ToString());
	}
	
	for(FName Name : RowNames)
	{
		FAvailableCharacter* AvailableCharacter = DT_AvailableCharacters->FindRow<FAvailableCharacter>(Name,TEXT("DT_AvailableCharacter"));

		if(AvailableCharacter)
		{
			if(UUW_CharacterSelectionItem* CharacterSelectionItem = CreateWidget<UUW_CharacterSelectionItem>(GetOwningPlayer(),CharacterSelectionItemClass))
			{
				// Check to see if players has unlocked character
				CharacterSelectionItem->Locked = (!UnlockedCharacterStrings.Contains(AvailableCharacter->CharacterName.ToString()) && AvailableCharacter->DefaultLocked);

				CharacterSelectionItem->Cost = AvailableCharacter->UnlockCost;

				CharacterSelectionItem->Description = AvailableCharacter->Description;

				CharacterSelectionItem->CharData = *AvailableCharacter;

				ScrollBox_Characters->AddChild(CharacterSelectionItem);

				// Setup dispatchers on button clicks
				CharacterSelectionItem->OnClicked.AddDynamic(this,&UUW_LobbyMenu::UpdateCharacterSelection);

				CharacterSelectionItem->OnPurchase.AddDynamic(this,&UUW_LobbyMenu::CharacterPurchased);
			}
		}
	}
}

void UUW_LobbyMenu::UpdatePlayerTotal(int32 Count)
{
	FText Text;
	if(Count>1)
		Text = FText::FromString(FString::Printf(TEXT("%d Players in Lobby"),Count));
	else
	{
		Text = FText::FromString(FString::Printf(TEXT("1 Player in Lobby")));
	}
	TextBlock_Players->SetText(Text);
}

void UUW_LobbyMenu::UpdateCharacterSelection(const FAvailableCharacter& CharData)
{
	if(GetOwningPlayerPawn()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		IInterface_CharacterManager::Execute_UpdateCharacterClass(GetOwningPlayerPawn(),CharData);

		if(IsValid(GameSave))
		{
			GameSave->Character = CharData;

			UFunctionLibrary_Helper::SavePlayerData(GetWorld(),GameSave);
		}
		else
		{
			RS_LOG_ERROR(TEXT("GameSave가 유효하지 않습니다."))
		}
		
	}
	else
	{
		RS_LOG_ERROR(TEXT("Pawn이 CharacterManager 인터페이스를 상속받지 않았습니다."))
	}
}

void UUW_LobbyMenu::CharacterPurchased(const FAvailableCharacter& PurchasedChar)
{
	if(GetOwningPlayerPawn()-GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		IInterface_CharacterManager::Execute_UpdateCharacterClass(GetOwningPlayerPawn(),PurchasedChar);
		if(IsValid(GameSave))
		{
			GameSave->Character = PurchasedChar;

			bool bCheckUnique = true;
			for(FText Name : GameSave->UnlockedCharacters)
			{
				if(Name.EqualTo(PurchasedChar.CharacterName))
				{
					bCheckUnique = false;
				}
			}
			if(bCheckUnique)
				GameSave->UnlockedCharacters.Add(PurchasedChar.CharacterName);

			UnlockedCharacters = GameSave->UnlockedCharacters;

			GameSave->Gold = GameSave->Gold - PurchasedChar.UnlockCost;

			UFunctionLibrary_Helper::SavePlayerData(GetWorld(),GameSave);

			CreateCharacterList();

			UpdateGold(GameSave->Gold);
		}
		else
		{
			RS_LOG_ERROR(TEXT("GameSave가 유효하지 않습니다."))
		}
	}
	else
	{
		RS_LOG_ERROR(TEXT("Pawn이 CharacterManager 인터페이스를 상속받지 않았습니다."))
	}
}

void UUW_LobbyMenu::BuildMapSelection()
{
	SetupMapDispatcher();

	TArray<FName> MapNames = DT_AvailableMaps->GetRowNames();

	for(FName Map : MapNames)
	{
		FAvailableMaps* AvailableMap = DT_AvailableMaps->FindRow<FAvailableMaps>(Map,TEXT("DT_AvailableMaps"));

		ComboBoxString_Maps->AddOption(AvailableMap->MapName.ToString());;
	}
	FAvailableMaps* AvailableMap = DT_AvailableMaps->FindRow<FAvailableMaps>(MapNames[0],TEXT("DT_AvailableMaps"));

	ComboBoxString_Maps->SetSelectedOption(AvailableMap->MapName.ToString());

}

void UUW_LobbyMenu::SetupMapDispatcher()
{
	ComboBoxString_Maps->OnSelectionChanged.AddDynamic(this,&UUW_LobbyMenu::OnMapSelected);
}

void UUW_LobbyMenu::OnMapSelected(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
	if(UBase_GameInstance* Base_GameInstance = Cast<UBase_GameInstance>(GetGameInstance()))
	{
		Base_GameInstance->CurrentMap = *SelectedItem;
	}
}


