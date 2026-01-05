// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_HealthBar.h"

#include "Components/ProgressBar.h"

UUW_HealthBar::UUW_HealthBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}



void UUW_HealthBar::RefreshHpBar()
{
	ProgressBar->SetPercent(CurrentHp/MaxHp);
}
