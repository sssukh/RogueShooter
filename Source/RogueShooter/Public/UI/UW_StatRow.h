// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_StatRow.generated.h"

struct FStatUIData;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_StatRow : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_StatRow(const FObjectInitializer& ObjectInitializer);
	
	void UpdateRow(FStatUIData StatUIData, float Value);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "StatRow | Designer ",meta = (BindWidget))
	TObjectPtr<UImage> StatIcon;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "StatRow | Designer ",meta = (BindWidget))
	TObjectPtr<UTextBlock> Description;
	
};
