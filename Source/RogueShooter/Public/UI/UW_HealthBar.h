// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interface_WidgetManager.h"
#include "UW_HealthBar.generated.h"

class UUnitWidgetController;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_HealthBar : public UUserWidget, public IInterface_WidgetManager
{
	GENERATED_BODY()
public:
	UUW_HealthBar(const FObjectInitializer& ObjectInitializer);
	
	void RefreshHpBar();
	
	virtual void SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController) override;
	
	UFUNCTION()
	void SetCurrentHealth(float NewValue);
	
	UFUNCTION()
	void SetMaxHealth(float NewValue);
public:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar;
	
	UPROPERTY()
	TObjectPtr<UUnitWidgetController> WidgetController;
	
	float CurrentHp;
	
	float MaxHp;
};
