// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ServerItem.generated.h"

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

public:
	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer", meta = (BindWidget))
	TObjectPtr<UButton> Button_Server;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Name;

	UPROPERTY(BlueprintReadWrite,Category="UW_ServerBrowser | Designer", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Players;

	// session 공부 필요 
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UW_ServerBrowser | Default", meta = (ExposeOnSpawn))
	// TObjectPtr<session>
};
