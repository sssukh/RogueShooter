// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InfoPopup.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnYesClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoClicked);

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_InfoPopup : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_InfoPopup(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonYesClicked();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_InfoPopup | Default", meta = (ExposeOnSpawn))
	FText DescriptionText;

	UPROPERTY(BlueprintReadWrite,Category="UW_InfoPopup | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Yes;

	UPROPERTY(BlueprintReadWrite,Category="UW_InfoPopup | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PopupText;

	FOnYesClicked OnYesClicked;

	FOnNoClicked OnNoClicked;
};
