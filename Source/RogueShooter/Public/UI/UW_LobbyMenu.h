// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LobbyMenu.generated.h"

class UUW_CharacterSelectionItem;
struct FAvailableCharacter;
enum class ECharacterClass : uint8;
class USG_Player;
class UTextBlock;
class USizeBox;
class UScrollBox;
class UImage;
class UComboBoxString;
class UButton;
class UBorder;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LobbyMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LobbyMenu(const FObjectInitializer ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnButtonLaunchClicked();

	// Load Player data
	// if none is found use default
	void LoadData();

	// Update gold amount in ui
	void UpdateGold(int32 Gold);

	// Only Host will be shown launch button - clients get updated text
	void HostCheck();

	void CreateCharacterList();

	// Update ui with Player total
	void UpdatePlayerTotal(int32 Count);

	// Update class name in lobby
	UFUNCTION()
	void UpdateCharacterSelection(const FAvailableCharacter& CharData);

	// we call for character to be updated based on new purchase,
	// then we add the purchased character to our save and update HUD, rebuilding character list with new data and update gold
	UFUNCTION()
	void CharacterPurchased(const FAvailableCharacter& PurchasedChar);

	// Build available maps from data table
	void BuildMapSelection();

	// setup dispatcher to update map
	void SetupMapDispatcher();

	// Set game instance map value
	// which we will read when host launches game
	UFUNCTION()
	void OnMapSelected(FString SelectedItem, ESelectInfo::Type SelectInfo);
public:
	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UBorder> Border_MapSelection;
	
	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Launch;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString_Maps;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Characters;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_ServerButton;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ClientStatus;
	
	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Gold;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Players;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_LobbyMenu | Default")
	TObjectPtr<USG_Player> GameSave;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_LobbyMenu | Default")
	TArray<ECharacterClass> LUnlockedCharacters;

	UPROPERTY(BlueprintReadWrite,Category="UW_LobbyMenu | Default")
	TArray<FText> UnlockedCharacters;

	UPROPERTY(Category="UW_LobbyMenu | DataTable")
	TObjectPtr<UDataTable> DT_AvailableCharacters;

	// TODO : 초기설정 
	UPROPERTY(Category="UW_LobbyMenu | DataTable")
	TObjectPtr<UDataTable> DT_AvailableMaps;
	
	// TODO : 초기설정 
	TSubclassOf<UUW_CharacterSelectionItem> CharacterSelectionItemClass;
};
