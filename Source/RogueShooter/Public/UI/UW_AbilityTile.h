// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_AbilityTile.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_AbilityTile : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_AbilityTile(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnIconLoaded_Internal(UObject* LoadedAsset); 

public:
	UPROPERTY(BlueprintReadWrite, Category="UW_AbilityTile | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadWrite, Category="UW_AbilityTile | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Level;

	// TODO : 로딩 아이콘으로 설정 
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_AbilityTile", meta = (ExposeOnSpawn))
	TObjectPtr<UTexture2D> LoadingIcon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UW_AbilityTile", meta = (ExposeOnSpawn))
	int32 Level = 0;

	UPROPERTY()
	TObjectPtr<UTexture2D> LoadedIcon;

	bool bIsLoaded = false;
};
