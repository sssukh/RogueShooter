// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_LevelUpMaster.h"

#include "Character/Base_Character.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "System/Subsystem/UIAssetCacheSubsystem.h"
#include "UI/UW_LevelUpCard.h"
#include "UI/UW_LevelUpItems.h"
#include "Utility/RSLog.h"

UUW_LevelUpMaster::UUW_LevelUpMaster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

	static ConstructorHelpers::FClassFinder<UUW_LevelUpCard> LevelupCardFinder(*AssetPath::Blueprint::WBP_LevelUpCard_C);
	if(LevelupCardFinder.Succeeded())
	{
		LevelUpCardClass = LevelupCardFinder.Class;
	}
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

// icon을 로딩중 이미지로 변경할 것(실제 사용할 이미지는 로드 후 적용할 것)
// 위젯 내에 softobjectptr (Icon to load)변수를 둘 것.
// expose on spawn 키워드를 두어서 여기서 위젯 생성할 때 soft 경로를 전달.
void UUW_LevelUpMaster::AddSelection(FText Name, int32 Level, FText Desc, TSoftObjectPtr<UTexture2D> Icon, EActiveAbilities AAbility,
	EPassiveAbilities PAbility, EAbilityType Type)
{
	// assetPath에 levelupCard 클래스 추가할것.
	if(UUW_LevelUpCard* LevelUpCard = CreateWidget<UUW_LevelUpCard>(GetOwningPlayer(),LevelUpCardClass))
	{
		LevelUpCard->Name = Name;
		LevelUpCard->Level = Level;
		LevelUpCard->Description = Desc;
		LevelUpCard->AAbility = AAbility;
		LevelUpCard->PAbility = PAbility;
		LevelUpCard->Type = Type;

		UW_LevelUpItems->VerticalBox_Items->AddChildToVerticalBox(LevelUpCard);

		LevelUpCard->OnSelected.AddDynamic(this,&UUW_LevelUpMaster::Close);

		UGameInstance* GameInstance = GetGameInstance();
		if(!GameInstance) return;

		UUIAssetCacheSubsystem* AssetCache = GameInstance->GetSubsystem<UUIAssetCacheSubsystem>();

		FOnAssetLoaded CardUICallback;

		CardUICallback.AddDynamic(LevelUpCard,&UUW_LevelUpCard::OnIconLoaded_Internal);

		// 애셋 요청 
		AssetCache->RequestAsset(Icon,CardUICallback);
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
