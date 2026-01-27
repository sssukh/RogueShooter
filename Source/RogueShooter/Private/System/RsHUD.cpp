// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsHUD.h"

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "System/RsWidgetController.h"
#include "UI/UW_PlayerHud.h"

URsWidgetController* ARsHUD::GetOverlayWidgetController(const FWidgetControllerParams& WcParams)
{
	// 싱글톤처럼 하나만 유지하거나, 필요시 생성
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<URsWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WcParams);
	}
	return OverlayWidgetController;
}

void ARsHUD::InitOverlay(const FWidgetControllerParams& WCParams)
{
	// 이미 생성되었는지 체크
	if (OverlayWidgetClass && OverlayWidgetControllerClass)
	{
		// 1. 위젯 컨트롤러 생성 및 데이터 주입
		// const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
		URsWidgetController* WidgetController = GetOverlayWidgetController(WCParams);

		// 2. 위젯(껍데기) 생성
		OverlayWidget = CreateWidget<UUW_PlayerHud>(GetWorld(), OverlayWidgetClass);
        
		// 3. ⭐ 연결 (핵심) ⭐
		// 위젯에게 "너의 데이터 담당자는 얘야"라고 알려줌
		OverlayWidget->SetWidgetController(WidgetController);
        
		// 4. 컨트롤러에게 "이제 감시 시작해!" 명령
		WidgetController->BindCallbacksToDependencies();

		// 5. 화면 부착
		OverlayWidget->AddToViewport();
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
