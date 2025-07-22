// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HealthBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_HealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_HealthBar(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
};
