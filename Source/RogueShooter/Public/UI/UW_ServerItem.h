// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "UW_ServerItem.generated.h"

class UUW_LoadingScreen;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ServerItem : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ServerItem(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonServerClicked();

	// TODO : 현재 JoinSession 후에 성공하여도 호출될 함수가 아무것도 설정되지 않음 
public:
	UPROPERTY(BlueprintReadWrite,Category="UW_ServerItem | Designer", meta = (BindWidget))
	TObjectPtr<UButton> Button_Server;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerItem | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Name;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerItem | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Players;
	
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_ServerBrowser | Default", meta = (ExposeOnSpawn))
	FOnlineSessionSearchResult SessionResult;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_ServerItem | Set up")
	TSubclassOf<UUW_LoadingScreen> LoadingScreenClass;
};
