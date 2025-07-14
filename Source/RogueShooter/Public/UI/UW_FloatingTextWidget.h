// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UW_FloatingTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_FloatingTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_FloatingTextWidget(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "UW Floating Text Widget", meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Damage;
};
