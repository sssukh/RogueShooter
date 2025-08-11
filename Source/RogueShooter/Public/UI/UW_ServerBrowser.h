// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ServerBrowser.generated.h"

class UUW_ServerItem;
class UTextBlock;
class UScrollBox;
class UImage;
class UComboBoxString;
class UCircularThrobber;
class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ServerBrowser : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ServerBrowser(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonCloseClicked();

	UFUNCTION()
	void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnButtonRefreshClicked();

	void RefreshServerList();
	
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SessionSearchResults, bool bWasSuccessful);
public:
	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Close;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UButton> Button_Refresh;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UCircularThrobber> CircularThrobber_Searching;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBoxString_Type;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UImage> Image;
	
	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Servers;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer",meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Status;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_ServerBrowser | Default")
	int32 SessionsToFind = 25;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="UW_ServerBrowser | Default")
	bool LAN = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_ServerBrowser | Set up")
	TSubclassOf<UUW_ServerItem> ServerItemClass;

	FDelegateHandle FindSessionDelegateHandle;
};
