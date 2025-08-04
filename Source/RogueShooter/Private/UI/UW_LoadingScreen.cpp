// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LoadingScreen.h"

#include "Components/TextBlock.h"

UUW_LoadingScreen::UUW_LoadingScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_LoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Info->SetText(Text);
}
