// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_MatchResults.h"

#include "Components/TextBlock.h"

UUW_MatchResults::UUW_MatchResults(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_MatchResults::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Result->SetText(Victory?FText::FromString(TEXT("VICTORY")):FText::FromString(TEXT("DEFEAT")));

	TextBlock_Kills->SetText(FText::FromString(FString::Printf(TEXT("Enemies killed : %d"),Kills)));
}
