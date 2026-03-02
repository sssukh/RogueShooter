// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_PlayerHud.h"

#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/RSEnumStruct.h"
#include "System/Subsystem/UIAssetCacheSubsystem.h"
#include "UI/UW_AbilityTile.h"
#include "UI/UW_HUDHealthBar.h"
#include "UI/UW_SkillSlotList.h"
#include "Utility/RSLog.h"

UUW_PlayerHud::UUW_PlayerHud(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUW_AbilityTile> AbilityTileFinder(*AssetPath::Blueprint::WBP_AbilityTile_C);
	if(AbilityTileFinder.Succeeded())
	{
		AbilityTileClass = AbilityTileFinder.Class;
	}
}

void UUW_PlayerHud::NativeConstruct()
{
	Super::NativeConstruct();

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
			UGameInstance* GameInstance = GetGameInstance();
			if(!GameInstance) return;

			UUIAssetCacheSubsystem* AssetCache = GameInstance->GetSubsystem<UUIAssetCacheSubsystem>();

			FOnAssetLoaded CardUICallback;

			CardUICallback.AddDynamic(AbilityTile,&UUW_AbilityTile::OnIconLoaded_Internal);

			// 애셋 요청 
			AssetCache->RequestAsset(UFunctionLibrary_Helper::FindActiveIcon(GetWorld(),active),CardUICallback);
			
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
			UGameInstance* GameInstance = GetGameInstance();
			if(!GameInstance) return;

			UUIAssetCacheSubsystem* AssetCache = GameInstance->GetSubsystem<UUIAssetCacheSubsystem>();

			FOnAssetLoaded CardUICallback;

			CardUICallback.AddDynamic(AbilityTile,&UUW_AbilityTile::OnIconLoaded_Internal);

			// 애셋 요청 
			AssetCache->RequestAsset(UFunctionLibrary_Helper::FindPassiveIcon(GetWorld(),passive),CardUICallback);
			
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

void UUW_PlayerHud::UpdateExpBar()
{
	ProgressBar_XP->SetPercent(CurrentExp/MaxExp);
}

void UUW_PlayerHud::SetLevel(float InLevel)
{
	TextBlock_Level->SetText(FText::FromString(FString::Printf(TEXT("Level %d"),(int32)InLevel)));
}

void UUW_PlayerHud::SetCurrentExp(float NewValue)
{
	CurrentExp = NewValue;
	
	UpdateExpBar();
}

void UUW_PlayerHud::SetMaxExp(float NewValue)
{
	MaxExp = NewValue;
	
	UpdateExpBar();
}


void UUW_PlayerHud::SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController)
{
	WidgetController = Cast<URsWidgetController>(InWidgetController);
	
	if (!WidgetController)
	{
		RS_LOG_WARNING(TEXT("Widget Controller가 설정되지 않았습니다."))
		return;
	}
	
	WidgetController->OnExpChanged.AddDynamic(this,&UUW_PlayerHud::SetCurrentExp);
	WidgetController->OnMaxExpChanged.AddDynamic(this,&UUW_PlayerHud::SetMaxExp);
	WidgetController->OnLevelChanged.AddDynamic(this,&UUW_PlayerHud::SetLevel);
	
	if (UUW_SkillSlotList* SkillSlotList = Cast<UUW_SkillSlotList>(Overlay_SkillSlotList->GetChildAt(0)))
	{
		SkillSlotList->SetWidgetController_Implementation(WidgetController);
	}
	
	if (UUW_HUDHealthBar* HealthBarSlot = Cast<UUW_HUDHealthBar>(Overlay_HealthBar->GetChildAt(0)))
	{
		HealthBarSlot->SetWidgetController_Implementation(WidgetController);
	}
	
	
	WidgetController->BroadcastInitialValues();
}





