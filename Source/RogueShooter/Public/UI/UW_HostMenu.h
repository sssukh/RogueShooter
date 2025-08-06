// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HostMenu.generated.h"

class UImage;
class UComboBoxString;
class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_HostMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_HostMenu(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnButtonHostClicked();

	UFUNCTION()
	void OnButtonCloseClicked();
	
public:
	UPROPERTY(BlueprintReadWrite,Category = "UW_HostMenu",meta=(BindWidget))
	TObjectPtr<UButton> Button_Close;

	UPROPERTY(BlueprintReadWrite,Category = "UW_HostMenu",meta=(BindWidget))
	TObjectPtr<UButton> Button_Host;

	UPROPERTY(BlueprintReadWrite,Category = "UW_HostMenu",meta=(BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString_LaN;

	UPROPERTY(BlueprintReadWrite,Category = "UW_HostMenu",meta=(BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString_Players;

	UPROPERTY(BlueprintReadWrite,Category = "UW_HostMenu",meta=(BindWidget))
	TObjectPtr<UImage> Image;

	UPROPERTY(BlueprintReadWrite,Category= "UW_HostMenu | Default")
	int32 MaxPlayers = 2;

	UPROPERTY(BlueprintReadWrite,Category= "UW_HostMenu | Default")
	bool LaN = false;
};
