// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LevelUpMaster.h"

#include "Character/Base_Character.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "UI/UW_LevelUpCard.h"
#include "UI/UW_LevelUpItems.h"
#include "Utility/RSLog.h"

UUW_LevelUpMaster::UUW_LevelUpMaster(const FObjectInitializer ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundBase> ChestSoundFinder(*AssetPath::Sound::ChestSound);

	if(ChestSoundFinder.Succeeded())
		ChestSound = ChestSoundFinder.Object;
	else
	{
		RS_LOG_ERROR(TEXT("chest sound 를 로드할 수 없습니다."))
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

void UUW_LevelUpMaster::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::PlaySound2D(GetWorld(),ChestSound);

	Button_Intro->OnClicked.AddDynamic(this,&UUW_LevelUpMaster::ButtonIntroClicked);
}

void UUW_LevelUpMaster::ResetUI()
{
	WidgetSwitcher_LevelUp->SetActiveWidgetIndex(0);

	ResetLevelUpItems();

	UGameplayStatics::PlaySound2D(GetWorld(),ChestSound);
}

void UUW_LevelUpMaster::ResetLevelUpItems()
{
	UW_LevelUpItems->VerticalBox_Items->ClearChildren();

	if(OnReady.IsBound())
		OnReady.Broadcast();
}

void UUW_LevelUpMaster::SetReference()
{
	AsBaseCharacter = Cast<ABase_Character>(GetOwningPlayerPawn());

	if(AsBaseCharacter == nullptr)
	{
		RS_LOG_ERROR(TEXT("AsBaseCharacter가 정상적으로 설정되지 않았습니다."))
	}
}

void UUW_LevelUpMaster::AddSelection(FText Name, int32 Level, FText Desc, UTexture2D* Icon, EActiveAbilities AAbility,
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

		UW_LevelUpItems->VerticalBox_Items->AddChildToVerticalBox(LevelUpCard);

		LevelUpCard->OnSelected.AddDynamic(this,&UUW_LevelUpMaster::Close);
	}
}

void UUW_LevelUpMaster::Close(EAbilityType Type, EActiveAbilities AAbility, EPassiveAbilities PAbility)
{
	if(OnClose.IsBound())
		OnClose.Broadcast(Type,AAbility,PAbility);

	RemoveFromParent();

	UGameplayStatics::PlaySound2D(GetWorld(),CloseMenuSound);
}

void UUW_LevelUpMaster::ButtonIntroClicked()
{
	WidgetSwitcher_LevelUp->SetActiveWidgetIndex(1);

	UGameplayStatics::PlaySound2D(GetWorld(),ChestGoldSound);
}
