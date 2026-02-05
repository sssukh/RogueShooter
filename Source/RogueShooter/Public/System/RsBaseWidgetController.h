// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RsBaseWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	// 생성자: 4개의 핵심 데이터를 받아서 초기화
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	// AttributeSet은 다음과 같이 ASC를 이용해서 세팅하기 
	// 초기화 시점 유의하기 
	/*
	void UMyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
	{
		AbilitySystemComponent = Params.AbilitySystemComponent;
		
		// ASC에게 달라고 하기 (자동 검색)
		if (AbilitySystemComponent)
		{
			// 주의: const UAttributeSet* 로 반환되므로 캐스팅 필요
			HealthSet = Cast<UHealthSet>(AbilitySystemComponent->GetAttributeSet(UHealthSet::StaticClass()));
			CombatSet = Cast<UCombatSet>(AbilitySystemComponent->GetAttributeSet(UCombatSet::StaticClass()));
			ExpSet = Cast<UExpSet>(AbilitySystemComponent->GetAttributeSet(UExpSet::StaticClass()));
		}
	}
	 */
};

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class ROGUESHOOTER_API URsBaseWidgetController : public UObject
{
	GENERATED_BODY()
public:
	// 초기화 파라미터 구조체 받기
	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

protected:
	// 모든 컨트롤러가 공통으로 필요한 레퍼런스 
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	
};
