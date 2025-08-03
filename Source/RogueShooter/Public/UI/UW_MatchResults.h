// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MatchResults.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_MatchResults : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_MatchResults(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Kills;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Result;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn))
	bool Victory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn))
	int32 Kills = 0;
};
