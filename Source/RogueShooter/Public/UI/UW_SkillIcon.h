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
 * 
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
	TObjectPtr<UTextBlock> SkillDescription;
	
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn))
	TObjectPtr<URsWidgetController> WidgetController;
	
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn))
	FGameplayTag SkillTag;
	// Skill tooltip도 필요 
};
