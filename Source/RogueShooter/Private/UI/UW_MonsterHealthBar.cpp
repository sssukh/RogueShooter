// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_MonsterHealthBar.h"

#include "Components/ProgressBar.h"

UUW_MonsterHealthBar::UUW_MonsterHealthBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_MonsterHealthBar::UpdateHealth(float InCurrent, float InMax)
{
	if (!HealthBar || InMax <= 0.0f) return;
	HealthBar->SetPercent(InCurrent/InMax);
}
