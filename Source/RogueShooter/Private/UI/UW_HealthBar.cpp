// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HealthBar.h"

#include "Components/ProgressBar.h"
#include "System/UnitWidgetController.h"
#include "Utility/RSLog.h"

UUW_HealthBar::UUW_HealthBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}



void UUW_HealthBar::RefreshHpBar()
{
	ProgressBar->SetPercent(CurrentHp/MaxHp);
}

void UUW_HealthBar::SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController)
{
	WidgetController = Cast<UUnitWidgetController>(InWidgetController);
	
	if (!WidgetController)
	{
		RS_LOG_WARNING(TEXT("Widget Controller가 설정되지 않았습니다."))
		return;
	}
	
	WidgetController->OnHealthChanged.AddDynamic(this,&UUW_HealthBar::SetCurrentHealth);
	WidgetController->OnMaxHealthChanged.AddDynamic(this,&UUW_HealthBar::SetMaxHealth);
	// TODO : shield 처리 로직 및 UI 추가 
	// WidgetController->OnShieldChanged
	WidgetController->BroadcastInitialValues();
}



void UUW_HealthBar::SetCurrentHealth(float NewValue)
{
	CurrentHp = NewValue;
	
	RefreshHpBar();
}

void UUW_HealthBar::SetMaxHealth(float NewValue)
{
	MaxHp = NewValue;
	
	RefreshHpBar();
}

