// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HUDHealthBar.h"

#include "Components/ProgressBar.h"
#include "System/RsWidgetController.h"
#include "System/UnitWidgetController.h"
#include "Utility/RSLog.h"

UUW_HUDHealthBar::UUW_HUDHealthBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}



void UUW_HUDHealthBar::RefreshHpBar()
{
	ProgressBar->SetPercent(CurrentHp/MaxHp);
}

void UUW_HUDHealthBar::SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController)
{
	WidgetController = Cast<URsWidgetController>(InWidgetController);
	
	if (!WidgetController)
	{
		RS_LOG_WARNING(TEXT("Widget Controller가 설정되지 않았습니다."))
		return;
	}
	
	WidgetController->OnHealthChanged.AddDynamic(this,&UUW_HUDHealthBar::SetCurrentHealth);
	WidgetController->OnMaxHealthChanged.AddDynamic(this,&UUW_HUDHealthBar::SetMaxHealth);
	WidgetController->OnShieldChanged.AddDynamic(this,&UUW_HUDHealthBar::SetShield);
	WidgetController->BroadcastInitialValues();
}



void UUW_HUDHealthBar::SetCurrentHealth(float NewValue)
{
	CurrentHp = NewValue;
	
	RefreshHpBar();
}

void UUW_HUDHealthBar::SetMaxHealth(float NewValue)
{
	MaxHp = NewValue;
	
	RefreshHpBar();
}

void UUW_HUDHealthBar::SetShield(float NewValue)
{
	Shield = NewValue;
}

