// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MonsterHealthBar.generated.h"

class ABase_Enemy;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_MonsterHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_MonsterHealthBar(const FObjectInitializer& ObjectInitializer);
	
	void UpdateHealth(float InCurrent,float InMax);
public:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<ABase_Enemy> TargetEnemy;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	double HideTimer;
	
	
	
};
