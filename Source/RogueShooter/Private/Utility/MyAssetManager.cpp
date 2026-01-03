// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MyAssetManager.h"

#include "Utility/FRsGameplayTags.h"

UMyAssetManager::UMyAssetManager()
{
}

void UMyAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FRsGameplayTags::InitializeNativeGameplayTags();
}
