// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ItemTooltip.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Data/Item/ItemData.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UI/UW_StatRow.h"

UUW_ItemTooltip::UUW_ItemTooltip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUW_ItemTooltip::UpdateTooltip(const UItemData* ItemData)
{
	if (!ItemData || !StatDataTable) return;
	
	// 1. 정보설정
	ItemName->SetText(ItemData->ItemName);
	ItemDescription->SetText(ItemData->Description);
	
	// 2. 스탯 컨테이너 초기화
	Statlist->ClearChildren();
	
	// 3. 아이템의 스탯 맵 순회
	for (const auto& Pair : ItemData->ItemStats)
	{
		FGameplayTag Tag = Pair.Key;
		float Value = Pair.Value;
		
		// 4. 데이터 테이블에서 태그에 맞는 UI 정보 찾기
		FStatUIData* FoundRow = FindStatUIDataByTag(Tag);
		
		if (FoundRow)
		{
			// 5. 스탯 한 줄 위젯 생성 및 추가
						UUW_StatRow* RowWidget = CreateWidget<UUW_StatRow>(this,StatRowWidgetClass);
			
			RowWidget->UpdateRow(*FoundRow,Value);
			Statlist->AddChild(RowWidget);
		}
		
	}
}

FStatUIData* UUW_ItemTooltip::FindStatUIDataByTag(FGameplayTag Tag)
{
	static const FString ContextString(TEXT("StatUIData Context"));
	TArray<FStatUIData*> AllRows;
	StatDataTable->GetAllRows<FStatUIData>(ContextString,AllRows);
	
	for (FStatUIData* Row : AllRows)
	{
		if (Row->StatTag.MatchesTag(Tag))
		{
			return Row;
		}
	}
	return nullptr;
}
