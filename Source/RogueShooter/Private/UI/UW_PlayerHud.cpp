// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_PlayerHud.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UI/UW_AbilityTile.h"

UUW_PlayerHud::UUW_PlayerHud(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_PlayerHud::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle delayHandle;
	GetWorld()->GetTimerManager().SetTimer(delayHandle,FTimerDelegate::CreateLambda([]()
	{
		
	}),2.0f,false);
}

void UUW_PlayerHud::BuildHotbar(const TMap<EActiveAbilities, int32>& ActiveAbilities,
	const TMap<EPassiveAbilities, int32>& PassiveAbilities)
{
	HorizontalBox_Active->ClearChildren();

	TArray<EActiveAbilities> AAbilities;
	ActiveAbilities.GetKeys(AAbilities);

	for(EActiveAbilities active : AAbilities)
	{
		UUW_AbilityTile* AbilityTile = CreateWidget<UUW_AbilityTile>(GetOwningPlayer(),AbilityTileClass);

		if(AbilityTile != nullptr)
		{
			AbilityTile->Icon = UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),active);
			AbilityTile->Level = *ActiveAbilities.Find(active);

			HorizontalBox_Active->AddChild(AbilityTile);
		}
	}

	HorizontalBox_Passives->ClearChildren();

	TArray<EPassiveAbilities> PAbilities;
	PassiveAbilities.GetKeys(PAbilities);

	for(EPassiveAbilities passive : PAbilities)
	{
		UUW_AbilityTile* AbilityTile = CreateWidget<UUW_AbilityTile>(GetOwningPlayer(),AbilityTileClass);

		if(AbilityTile != nullptr)
		{
			AbilityTile->Icon = UFunctionLibrary_Helper::FindPassiveIcon(GetWorld(),passive);
			AbilityTile->Level = *PassiveAbilities.Find(passive);

			HorizontalBox_Passives->AddChild(AbilityTile);
		}
	}
}



void UUW_PlayerHud::UpdateGold(int32 Gold)
{
	TextBlock_Gold->SetText(FText::FromString(FString::Printf(TEXT("%d"),Gold)));
}

void UUW_PlayerHud::UpdateTime(FText Time)
{
	TextBlock_Time->SetText(Time);
}



