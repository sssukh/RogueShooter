// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interface_WidgetManager.h"
#include "UW_ReticleWidgetBase.generated.h"

class URsWidgetController;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_ReticleWidgetBase : public UUserWidget, public IInterface_WidgetManager
{
	GENERATED_BODY()
public:
	// 무기나 캐릭터 포인터를 받아서 연결하는 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Reticle")
	void InitializeReticle(AActor* InWeaponOrCharacter);

	virtual void SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController) override;
	
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 🌟 블루프린트에서 구현할 이벤트 (C++에서는 호출만 함)
	// 탄퍼짐 값을 던져주면, BP에서 이 값을 바탕으로 이미지를 벌립니다.
	UFUNCTION(BlueprintImplementableEvent, Category = "Reticle")
	void OnSpreadUpdated(double CurrentSpread);

	
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Reticle")
	void OnHitEnemy();
	
protected:
	UPROPERTY()
	TObjectPtr<URsWidgetController> WidgetController;
	
	// 탄퍼짐 데이터를 읽어올 대상
	UPROPERTY()
	AActor* TargetActor;
};
