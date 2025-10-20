// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LevelUpMaster.generated.h"

class UUW_LevelUpCard;
enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;
enum class EAbilityType : uint8;
class UUW_LevelUpItems;
class ABase_Character;
class UWidgetSwitcher;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReadyLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCloseLevelUp,EAbilityType,Type,EActiveAbilities,AAbilities,EPassiveAbilities,PAbilities);

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LevelUpMaster : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LevelUpMaster(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void ResetUI();

	void ResetLevelUpItems();

	void SetReference();

	void AddSelection(FText Name,int32 Level, FText Desc, TSoftObjectPtr<UTexture2D> Icon, EActiveAbilities AAbility, EPassiveAbilities PAbility, EAbilityType Type);

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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UUW_LevelUpMaster | Set up")
	TSubclassOf<UUW_LevelUpCard> LevelUpCardClass;
	
	FOnReadyLevelUp OnReady;

	FOnCloseLevelUp OnClose;
};
