// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UW_CharacterSelectionItem.generated.h"

class UUW_InfoPopup;
class USG_Player;
class UTextBlock;
class UButton;
struct FButtonStyle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedCharacterSelection,const FAvailableCharacter&,CharData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPurchase,const FAvailableCharacter&,PurchasedChar);



/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_CharacterSelectionItem : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_CharacterSelectionItem(const FObjectInitializer& ObjectInitializer);

	// Build Item Card
	virtual void NativeConstruct() override;

	// Purchase Flow - Ensure player has enough gold to purchase char
	// then create confirmation window
	void PurchaseFlow();

	UFUNCTION()
	void PurchaseCharacter();

	UFUNCTION()
	void OnButtonClicked();
public:
	UPROPERTY(BlueprintReadWrite,Category="UW_CharacterSelectionItem | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(BlueprintReadWrite,Category="UW_CharacterSelectionItem | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Character;

	UPROPERTY(BlueprintReadWrite,Category="UW_CharacterSelectionItem | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default",meta = (ExposeOnSpawn))
	bool Locked = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default",meta = (ExposeOnSpawn))
	int32 Cost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default",meta = (ExposeOnSpawn))
	FText Description;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default")
	FButtonStyle LockedStyle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default",meta = (ExposeOnSpawn))
	FAvailableCharacter CharData;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default")
	TObjectPtr<USG_Player> SaveData;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Default")
	FText CharacterName;

	FOnClickedCharacterSelection OnClicked;

	FOnPurchase OnPurchase;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_CharacterSelectionItem | Set up")
	TSubclassOf<UUW_InfoPopup> InfoPopupClass;
};
