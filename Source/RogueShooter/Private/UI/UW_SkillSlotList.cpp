// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_SkillSlotList.h"

#include "Components/HorizontalBox.h"
#include "UI/UW_SkillIcon.h"

UUW_SkillSlotList::UUW_SkillSlotList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_SkillSlotList::NativeConstruct()
{
	Super::NativeConstruct();
	
	HorizontalBox_SkillSlot->ClearChildren();
	
	if (!SkillIconClass)
		return;
	
	for (int i=0;i<SkillTagsforSlots.Num();++i)
	{
		UUW_SkillIcon* SkillSlot = CreateWidget<UUW_SkillIcon>(GetOwningPlayer(),SkillIconClass);
		
		if (!SkillSlot)
			return;
		
		SkillSlot->WidgetController = WidgetController;
		
		SkillSlot->SkillTag = SkillTagsforSlots[i];
		
		HorizontalBox_SkillSlot->AddChildToHorizontalBox(SkillSlot);
	}
}
