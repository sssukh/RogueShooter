// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LevelUpCard.generated.h"


enum class EAbilityType : uint8;
enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;
class UTextBlock;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSelected,EAbilityType,Type,EActiveAbilities,AAbility,EPassiveAbilities,PAbility);

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LevelUpCard : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LevelUpCard(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ButtonOnClicked();

	UFUNCTION()
	void LoadIconAsynchronous(const TSoftObjectPtr<UTexture2D>& IconToLoad);
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "UW_LevelUpCard | Designer", meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(BlueprintReadWrite, Category = "UW_LevelUpCard | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadWrite, Category = "UW_LevelUpCard | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Desc;

	UPROPERTY(BlueprintReadWrite, Category = "UW_LevelUpCard | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	UPROPERTY(BlueprintReadWrite, Category = "UW_LevelUpCard | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	FText Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	int32 Level;

	// TODO : 삭제 
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	EActiveAbilities AAbility;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	EPassiveAbilities PAbility;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member", meta = (ExposeOnSpawn))
	EAbilityType Type;

	FOnSelected OnSelected;
	
	// TODO : 블루프린트에서 로딩 이미지 설정 후 다시 코드에서 설정해보기 
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member")
	TObjectPtr<UTexture2D> LoadingIcon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_LevelUpCard | Member")
	TObjectPtr<UTexture2D> LoadedIcon;
};
