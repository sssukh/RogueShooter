// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LevelUpMaster.generated.h"

enum class EPassiveAbilities : uint8;
enum class EActiveAbilities;
enum class EAbilityType;
class UUW_LevelUpItems;
class ABase_Character;
class UWidgetSwitcher;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReady)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClose)

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LevelUpMaster : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LevelUpMaster(const FObjectInitializer ObjectInitializer);

	virtual void NativeConstruct() override;

	void ResetUI();

	void ResetLevelUpItems();

	void SetReference();

	void AddSelection(FText Name,int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility, EPassiveAbilities PAbility, EAbilityType Type);

	UFUNCTION()
	void Close(EAbilityType Type, EActiveAbilities AAbility, EPassiveAbilities PAbility);

	UFUNCTION()
	void ButtonIntroClicked();
public:

public:
	UPROPERTY(BlueprintReadWrite,Category = "UUW_LevelUpMaster | Designer", meta = (BindWidget))
	TObjectPtr<UButton> Button_Intro;

	UPROPERTY(BlueprintReadWrite,Category = "UUW_LevelUpMaster | Designer", meta = (BindWidget))
	TObjectPtr<UUW_LevelUpItems> UW_LevelUpItems;
	
	UPROPERTY(BlueprintReadWrite,Category = "UUW_LevelUpMaster | Designer", meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_LevelUp;

	UPROPERTY(BlueprintReadWrite,Category = "UUW_LevelUpMaster | Variable")
	TObjectPtr<ABase_Character> AsBaseCharacter;

	UPROPERTY()
	TObjectPtr<USoundBase> ChestSound;

	UPROPERTY()
	TObjectPtr<USoundBase> CloseMenuSound;

	UPROPERTY()
	TObjectPtr<USoundBase> ChestGoldSound;

	
	TSubclassOf<UUW_LevelUpCard> LevelUpCardClass;
	
	FOnReady OnReady;

	FOnClose OnClose;
};
