// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Interface_WidgetManager.h"
#include "UW_SkillSlotList.generated.h"

struct FGameplayTag;
class URsWidgetController;
class UUW_SkillIcon;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUW_SkillSlotList : public UUserWidget, public IInterface_WidgetManager
{
	GENERATED_BODY()
public:
	UUW_SkillSlotList(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	
	void SetSkillTagsForSlots(const TArray<FGameplayTag>& SkillTags);
	
	virtual void SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController) override;
public:
	UPROPERTY(BlueprintReadWrite,Category = "SKill Slot List", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_SkillSlot;
	
	UPROPERTY()
	TArray<UUW_SkillIcon*> SkillIcons;
	
	UPROPERTY()
	TObjectPtr<URsWidgetController> WidgetController;
	
protected:
	UPROPERTY(BlueprintReadOnly,Category="SKill Slot List")
	TArray<FGameplayTag> SkillTagsForSlots;
};
