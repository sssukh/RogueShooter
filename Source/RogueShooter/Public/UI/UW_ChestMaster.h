// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ChestMaster.generated.h"

class UUW_LevelUpCard;
enum class EAbilityType : uint8;
enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClose);

class UWidgetSwitcher;
class UUW_ChestItems;
class UButton;
class ABase_Character;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ChestMaster : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ChestMaster(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void ResetUI();

	UFUNCTION()
	void CloseChestUI();

	UFUNCTION()
	void ButtonIntroClicked();

	void ResetLevelUpItems();

	void ResetAbilitiesBox();

	void SetReferences();

	void ResetLevelupItems();

	void AddSelection(FText Name,int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility, EPassiveAbilities PAbility, EAbilityType Type);

public:
	UPROPERTY()
	TObjectPtr<ABase_Character> AsBaseCharacter;

	UPROPERTY()
	int32 Gold;

	UPROPERTY()
	TObjectPtr<UButton> Button_Intro;

	UPROPERTY()
	TObjectPtr<UUW_ChestItems> UW_ChestItems;

	UPROPERTY()
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_LevelUp;

	FOnReady OnReady;

	FOnClose OnClose;

	UPROPERTY()
	TObjectPtr<USoundBase> ChestSound;

	UPROPERTY()
	TObjectPtr<USoundBase> CloseMenuSound;

	UPROPERTY()
	TObjectPtr<USoundBase> ChestGoldSound;

	TSubclassOf<UUW_LevelUpCard> LevelUpCardClass;
};
