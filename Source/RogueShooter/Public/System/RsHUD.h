// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "RsHUD.generated.h"

struct FWidgetControllerParams;
class UAttributeSet;
class UUW_PlayerHud;
class UAbilitySystemComponent;
class URsWidgetController;



/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API ARsHUD : public AHUD
{
	GENERATED_BODY()
public:
	// 외부에서 접근할 수 있도록 Getter 제공
	URsWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WcParams);

	void InitOverlay(const FWidgetControllerParams& WCParams);

	// Overlay 조작 함수 
	void UpdateOverlayExpBar(float percent);
	
	void UpdateOverlayLevelUI(int32 Level);
	
	void UpdateOverlayTime(const FText& Time);
	
	void UpdateOverlayGold(int32 Amount);
	
protected:
	// 1. 메인 화면 위젯 클래스 (블루프린트에서 설정 WBP_Overlay)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUW_PlayerHud> OverlayWidgetClass;

	// 2. 위젯 컨트롤러 클래스 (블루프린트에서 설정 BP_OverlayWidgetController)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<URsWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditAnywhere,Category = "Config") 
	FGameplayTagContainer TagsToListen;

private:
	UPROPERTY()
	TObjectPtr<UUW_PlayerHud> OverlayWidget;

	UPROPERTY()
	TObjectPtr<URsWidgetController> OverlayWidgetController;
	
};
