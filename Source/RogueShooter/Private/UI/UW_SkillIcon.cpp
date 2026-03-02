// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_SkillIcon.h"

#include "Components/Image.h"
#include "System/RsWidgetController.h"

UUW_SkillIcon::UUW_SkillIcon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_SkillIcon::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_SkillIcon::ReceiveSkillInfo(const FGameplayTag& ReceivedMatchTag, const FSkillInfo& RowData)
{
	if (CooldownTag == ReceivedMatchTag)
	{
		SkillIcon->SetBrushFromTexture(RowData.SkillIcon);
		
		AbilityTag = RowData.AbilityTag;
	}
}

void UUW_SkillIcon::SetWidgetController_Implementation(URsBaseWidgetController* InWidgetController)
{
	URsWidgetController* IconController = Cast<URsWidgetController>(InWidgetController);
	
	if (IconController)
	{
		WidgetController = IconController;
		BP_InitializeCooldownEffect();
		IconController->OnSkillInfoLoaded.AddDynamic(this,&UUW_SkillIcon::ReceiveSkillInfo);
	}
}
