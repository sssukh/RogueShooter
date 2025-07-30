// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ChestMaster.h"

#include <string>

#include "Character/Base_Character.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_ChestItems.h"
#include "Utility/RSLog.h"
#include "UI/UW_LevelUpCard.h"

UUW_ChestMaster::UUW_ChestMaster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(*AssetPath::Sound::ChestSound);
	if(SoundFinder.Succeeded())
	{
		ChestSound = SoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> CloseMenuSoundFinder(*AssetPath::Sound::CloseMenuSound);
	if(CloseMenuSoundFinder.Succeeded())
	{
		CloseMenuSound = CloseMenuSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ChestGoldSoundFinder(*AssetPath::Sound::ChestGoldSound);
	if(ChestGoldSoundFinder.Succeeded())
	{
		ChestGoldSound = ChestGoldSoundFinder.Object;
	}

	// TODO : LevelUpCardClass 로드하기
}

void UUW_ChestMaster::NativeConstruct()
{
	Super::NativeConstruct();

	Gold = Gold + FMath::RandRange(1,50);

	UW_ChestItems->Button_Close->OnPressed.AddDynamic(this,&UUW_ChestMaster::CloseChestUI);

	UW_ChestItems->TextBlock_Gold->SetText(FText::FromString(std::to_string(Gold).c_str()));

	UGameplayStatics::PlaySound2D(GetWorld(),ChestSound);

	Button_Intro->OnClicked.AddDynamic(this,&UUW_ChestMaster::ButtonIntroClicked);
}

void UUW_ChestMaster::ResetUI()
{
	Gold = Gold + FMath::RandRange(1,50);

	WidgetSwitcher_LevelUp->SetActiveWidgetIndex(0);

	ResetLevelUpItems();

	UGameplayStatics::PlaySound2D(GetWorld(),ChestSound);

	UW_ChestItems->TextBlock_Gold->SetText(FText::FromString(std::to_string(Gold).c_str()));

}

void UUW_ChestMaster::CloseChestUI()
{
	if(OnClose.IsBound())
		OnClose.Broadcast();

	RemoveFromParent();

	UGameplayStatics::PlaySound2D(GetWorld(),CloseMenuSound);
}

void UUW_ChestMaster::ButtonIntroClicked()
{
	WidgetSwitcher_LevelUp->SetActiveWidgetIndex(1);

	UGameplayStatics::PlaySound2D(GetWorld(),ChestGoldSound);
}

void UUW_ChestMaster::ResetLevelUpItems()
{
	UW_ChestItems->VerticalBox_Items->ClearChildren();

	if(OnReady.IsBound())
		OnReady.Broadcast();
}

void UUW_ChestMaster::ResetAbilitiesBox()
{
}

void UUW_ChestMaster::SetReferences()
{
	AsBaseCharacter = Cast<ABase_Character>(GetOwningPlayerPawn());

	if(AsBaseCharacter == nullptr)
	{
		RS_LOG_ERROR(TEXT("AsBaseCharacter가 정상적으로 설정되지 않았습니다."))
	}
}

void UUW_ChestMaster::ResetLevelupItems()
{
	UW_ChestItems->VerticalBox_Items->ClearChildren();

	if(OnReady.IsBound())
		OnReady.Broadcast();
}

void UUW_ChestMaster::AddSelection(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
                                   EPassiveAbilities PAbility, EAbilityType Type)
{
	if(UUW_LevelUpCard* LevelUpCard = CreateWidget<UUW_LevelUpCard>(GetOwningPlayer(),LevelUpCardClass))
	{
		LevelUpCard->Name = Name;
		LevelUpCard->Level = Level;
		LevelUpCard->Description = Desc;
		LevelUpCard->Icon = Icon;
		LevelUpCard->AAbility = AAbility;
		LevelUpCard->PAbility = PAbility;
		LevelUpCard->Type = Type;

		UW_ChestItems->VerticalBox_Items->AddChildToVerticalBox(LevelUpCard);

		LevelUpCard->Button->SetIsEnabled(false);
	}
}
