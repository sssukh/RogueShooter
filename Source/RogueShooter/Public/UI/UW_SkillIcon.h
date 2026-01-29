// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UW_SkillIcon.generated.h"

class URsWidgetController;
class UImage;
class UTextBlock;


/**
 * 스킬의 이미지와 쿨다운 여부를 알려주는 아이콘
 * BP에서 설정한 CooldownTag와 WidgetController 내부에 설정한 Tag와 일치하면
 * 해당 Tag가 ASC에 부착되면 쿨다운이 동작한다.
 */
UCLASS()
class ROGUESHOOTER_API UUW_SkillIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_SkillIcon(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UImage> SkillCoolDown;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UImage> SkillIcon;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillCooldownText;
	
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn))
	TObjectPtr<URsWidgetController> WidgetController;
	
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn))
	FGameplayTag CooldownTag;
	// Skill tooltip도 필요 
};
