// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LoadingScreen.generated.h"

class UTextBlock;
class UImage;
class UCircularThrobber;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_LoadingScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LoadingScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite,Category="UUW_LoadingScreen | Designer", meta = (BindWidget))
	TObjectPtr<UCircularThrobber> CircularThrobber;

	UPROPERTY(BlueprintReadWrite,Category="UUW_LoadingScreen | Designer", meta = (BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite,Category="UUW_LoadingScreen | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Info;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category ="UUW_LoadingScreen | Default",meta = (ExposeOnSpawn))
	FText Text;
};
