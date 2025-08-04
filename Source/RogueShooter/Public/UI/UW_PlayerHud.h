// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlayerHud.generated.h"

class UUW_AbilityTile;
enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;
class UTextBlock;
class UProgressBar;
class UImage;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_PlayerHud : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_PlayerHud(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void BuildHotbar(const TMap<EActiveAbilities,int32>& ActiveAbilities, const TMap<EPassiveAbilities,int32>& PassiveAbilities);

	void UpdateGold(int32 Gold);

	void UpdateTime(FText Time);
	
public:
	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Active;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Passive;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_XP;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Gold;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(BlueprintReadWrite,Category = "UW_PlayerHud | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time;

	TSubclassOf<UUW_AbilityTile> AbilityTileClass;
};
