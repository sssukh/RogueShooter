// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsHUD.h"

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/RsCharacterInfo.h"
#include "Data/Attribute/HealthSet.h"
#include "Enemies/Base_Enemy.h"
#include "System/Gameplay_PlayerController.h"
#include "System/RsWidgetController.h"
#include "UI/UW_PlayerHud.h"
#include "Utility/RSLog.h"
#include "UI/UW_MonsterHealthBar.h"

URsWidgetController* ARsHUD::GetOverlayWidgetController(const FWidgetControllerParams& WcParams)
{
	// 싱글톤처럼 하나만 유지하거나, 필요시 생성
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<URsWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetTagsToListen(TagsToListen);
		OverlayWidgetController->SetWidgetControllerParams(WcParams);
	}
	return OverlayWidgetController;
}

void ARsHUD::InitOverlay(const FWidgetControllerParams& WCParams)
{
	// 이미 생성되었는지 체크
	if (OverlayWidget)
	{
		return;
	}
	
	if (OverlayWidgetClass && OverlayWidgetControllerClass)
	{
		// 1. 위젯 컨트롤러 생성 및 데이터 주입
		// const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
		URsWidgetController* WidgetController = GetOverlayWidgetController(WCParams);

		APlayerController* PC = GetOwningPlayerController();
		
		// 2. 위젯(껍데기) 생성
		OverlayWidget = CreateWidget<UUW_PlayerHud>(PC, OverlayWidgetClass);
		
		// 4. 컨트롤러에게 Attribute 값변화에 따른 델리게이트 발송 감시 명령
		WidgetController->BindCallbacksToDependencies();
		
		OverlayWidget->ReticleWidgetClass = Cast<AGameplay_PlayerController>(GetOwningPlayerController())->CharacterInfo->ReticleWidgetClass;
		
		// 6. 화면 부착
		OverlayWidget->AddToViewport();
		
		// 3. ⭐ 연결 (핵심) ⭐
		// 위젯에게 "너의 데이터 담당자는 얘야"라고 알려줌
		IInterface_WidgetManager::Execute_SetWidgetController(OverlayWidget,WidgetController);
		
		OverlayWidget->CreateReticle();
		// 5. 스킬 정보 싹 읽어서 방송 
		// OverlayWidgetController->BroadcastInitialAbilityInfo();
	}
}

void ARsHUD::UpdateOverlayExpBar(float Percent)
{
	OverlayWidget->ProgressBar_XP->SetPercent(Percent);
}

void ARsHUD::UpdateOverlayLevelUI(int32 Level)
{
	OverlayWidget->TextBlock_Level->SetText(FText::FromString(FString::Printf(TEXT("Level %d"),Level)));
}

void ARsHUD::UpdateOverlayTime(const FText& Time)
{
	OverlayWidget->UpdateTime(Time);
}

void ARsHUD::UpdateOverlayGold(int32 Amount)
{
	OverlayWidget->UpdateGold(Amount);
}

void ARsHUD::UpdateMonsterHealthBar(ABase_Enemy* InEnemy, float NewValue)
{
	UUW_MonsterHealthBar* AvailableBar = nullptr;

	// 케이스 A: 이미 이 몬스터를 담당하고 있는 체력바가 있는지 확인
	for (UUW_MonsterHealthBar* Bar : HealthBarPool)
	{
		if (Bar->TargetEnemy == InEnemy)
		{
			AvailableBar = Bar;
			break;
		}
	}

	// 케이스 B: 없다면, 창고에서 쉬고 있는(TargetMonster가 없는) 체력바 하나를 꺼냄
	if (!AvailableBar)
	{
		for (UUW_MonsterHealthBar* Bar : HealthBarPool)
		{
			if (Bar->TargetEnemy == nullptr)
			{
				AvailableBar = Bar;
				break;
			}
		}
	}

	if(AvailableBar == nullptr)
	{
		UUW_MonsterHealthBar* MonsterHealthBar = CreateWidget<UUW_MonsterHealthBar>(GetOwningPlayerController(),MonsterHealthBarClass);
		MonsterHealthBar->SetDesiredSizeInViewport(FVector2D(50.0f,10.0f));
		MonsterHealthBar->SetAlignmentInViewport(FVector2D(0.5f,0.5f));
		MonsterHealthBar->AddToViewport();
		MonsterHealthBar->SetVisibility(ESlateVisibility::Hidden);
		HealthBarPool.Add(MonsterHealthBar);
		
		AvailableBar = MonsterHealthBar;
	}

	// 체력바 갱신 및 화면에 표시
	if (AvailableBar)
	{
		AvailableBar->TargetEnemy = InEnemy;
		AvailableBar->UpdateHealth(InEnemy->AbilitySystemComponent->GetNumericAttribute(UHealthSet::GetHealthAttribute()), InEnemy->AbilitySystemComponent->GetNumericAttribute(UHealthSet::GetMaxHealthAttribute()));
		AvailableBar->HideTimer = 0.0f; // 타이머 초기화
		AvailableBar->SetVisibility(ESlateVisibility::HitTestInvisible); // 마우스 클릭 무시하고 화면에 표시
	}
}

void ARsHUD::HideMonsterHealthBar(AActor* Monster)
{
	if (!Monster) return;

	for (UUW_MonsterHealthBar* Bar : HealthBarPool)
	{
		// 이 몬스터를 담당하던 체력바를 찾아냈다면?
		if (Bar->TargetEnemy == Monster)
		{
			// 🌟 즉시 타겟을 비우고(풀로 반환) 화면에서 숨깁니다!
			Bar->TargetEnemy = nullptr;
			Bar->SetVisibility(ESlateVisibility::Collapsed);
			break; // 하나 찾았으니 더 이상 찾을 필요 없음
		}
	}
}

void ARsHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MonsterHealthBarClass)
	{
		for (int32 i =0;i<HealthBarPoolSize;++i)
		{
			UUW_MonsterHealthBar* MonsterHealthBar = CreateWidget<UUW_MonsterHealthBar>(GetOwningPlayerController(),MonsterHealthBarClass);
			MonsterHealthBar->SetDesiredSizeInViewport(FVector2D(50.0f,10.0f));
			MonsterHealthBar->SetAlignmentInViewport(FVector2D(0.5f,0.5f));
			MonsterHealthBar->AddToViewport();
			MonsterHealthBar->SetVisibility(ESlateVisibility::Hidden);
			HealthBarPool.Add(MonsterHealthBar);
		}
	}
}

void ARsHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	for (UUW_MonsterHealthBar* Bar : HealthBarPool)
	{
		// 일하고 있는 체력바만 연산합니다.
		if (Bar->TargetEnemy)
		{
			// 타이머 체크: 3초가 지나면 몬스터와의 연결을 끊고 숨깁니다.
			Bar->HideTimer += DeltaSeconds;
			if (Bar->HideTimer > 3.0f)
			{
				Bar->TargetEnemy = nullptr;
				Bar->SetVisibility(ESlateVisibility::Collapsed);
				continue;
			}

			// 🌟 [마법의 핵심] 3D 좌표를 2D 화면 좌표로 변환
			FVector2D ScreenPosition;
			// 몬스터 위치에서 Z축으로 100만큼 위로(머리 위) 띄웁니다.
			FVector TargetLocation = Bar->TargetEnemy->GetActorLocation() + FVector(0.f, 0.f, 60.f); 

			// ProjectWorldLocationToScreen은 카메라 뒤에 있으면 false를 반환합니다.
			bool bIsOnScreen = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, TargetLocation, ScreenPosition, false);

			if (bIsOnScreen)
			{
				// 위젯을 해당 화면 좌표로 이동시킵니다.
				Bar->SetPositionInViewport(ScreenPosition, false);
                
				if (Bar->GetVisibility() != ESlateVisibility::HitTestInvisible)
					Bar->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				// 몬스터가 내 등 뒤(카메라 밖)에 있다면 체력바를 잠시 숨깁니다.
				Bar->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
