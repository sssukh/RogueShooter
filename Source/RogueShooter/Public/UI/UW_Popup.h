// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_Popup.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnYesClickedPopup);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoClickedPopup);

UCLASS()
class ROGUESHOOTER_API UUW_Popup : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_Popup(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonYesClicked();

	UFUNCTION()
	void OnButtonNoClicked();
public:
	UPROPERTY(BlueprintReadWrite,Category= "UW_Popup | Designer",meta=(BindWidget))
	TObjectPtr<UButton> Button_No;

	UPROPERTY(BlueprintReadWrite,Category= "UW_Popup | Designer",meta=(BindWidget))
	TObjectPtr<UButton> Button_Yes;

	UPROPERTY(BlueprintReadWrite,Category= "UW_Popup | Designer",meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PopupText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_Popup | Default",meta = (ExposeOnSpawn))
	FText DescriptionText;

	FOnYesClickedPopup OnYesClicked;

	FOnNoClickedPopup OnNoClicked;
};
