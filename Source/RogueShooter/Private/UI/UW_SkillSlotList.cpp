// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_SkillSlotList.h"

#include "Components/HorizontalBox.h"
#include "System/RsWidgetController.h"
#include "UI/UW_SkillIcon.h"

UUW_SkillSlotList::UUW_SkillSlotList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_SkillSlotList::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_SkillSlotList::SetSkillTagsForSlots(const TArray<FGameplayTag>& SkillTags)
{
	SkillTagsForSlots = SkillTags;
}

void UUW_SkillSlotList::SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController)
{
	WidgetController = Cast<URsWidgetController>(InWidgetController);
	
	SetSkillTagsForSlots(WidgetController->GetTagsToListen());
	
	if (HorizontalBox_SkillSlot)
	{
		SkillIcons.Empty();
		
		for (int i=0;i<SkillTagsForSlots.Num();++i)
		{
			UUW_SkillIcon* SkillSlot = Cast<UUW_SkillIcon>(HorizontalBox_SkillSlot->GetChildAt(i));
		
			if (!SkillSlot)
				return;
		
			SkillIcons.Add(SkillSlot);
			
			SkillSlot->CooldownTag = SkillTagsForSlots[i];
			
			SkillSlot->SetWidgetController_Implementation(WidgetController);
		}
		
		WidgetController->BroadcastInitialAbilityInfo();
	}
}

