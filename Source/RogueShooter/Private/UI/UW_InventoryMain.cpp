// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InventoryMain.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "RogueShooter/AssetPath.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UI/UW_ItemIcon.h"
#include "UI/UW_ItemTooltip.h"


UUW_InventoryMain::UUW_InventoryMain(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUW_ItemIcon> IconClass(*AssetPath::Blueprint::WBP_ItemIcon_C);
	
	if (IconClass.Succeeded())
	{
		ItemIconClass = IconClass.Class;
	}
	
}

void UUW_InventoryMain::NativeConstruct()
{
	Super::NativeConstruct();
	
	ItemTooltip = CreateWidget<UUW_ItemTooltip>(this,ItemTooltipClass);
	
	ItemTooltip->AddToViewport();
	
	ItemTooltip->SetVisibility(ESlateVisibility::Hidden);
}

void UUW_InventoryMain::RefreshInventory()
{
	if (!GridPanel || !PlayerInventoryComponent.IsValid()) return;
	
	GridPanel->ClearChildren();
	
	for (int32 i = 0; i < PlayerInventoryComponent->InventorySlots.Num(); ++i)
	{
		UUW_ItemIcon* NewSlot = CreateWidget<UUW_ItemIcon>(this,ItemIconClass);
		
		NewSlot->InventoryMain = this;
		
		NewSlot->SetItemData(PlayerInventoryComponent->InventorySlots[i]);
		
		GridPanel->AddChildToUniformGrid(NewSlot,i/5,i%5);
	}
}

void UUW_InventoryMain::InitInventory(UInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent) return;
	
	PlayerInventoryComponent = InventoryComponent;
	
	PlayerInventoryComponent->OnInventoryUpdated.RemoveDynamic(this,&UUW_InventoryMain::RefreshInventory);
	
	PlayerInventoryComponent->OnInventoryUpdated.AddDynamic(this,&UUW_InventoryMain::RefreshInventory);
	
	RefreshInventory();
}

void UUW_InventoryMain::ShowItemTooltip(const UItemData* ItemData)
{
	if (!ItemData) return;
	
	// 위젯이 존재하지 않는다면 생성 및 viewport에 추가
	if (!ItemTooltip)
	{
		ItemTooltip = CreateWidget<UUW_ItemTooltip>(this,ItemTooltipClass);
		
		ItemTooltip->UpdateTooltip(ItemData);
		
		ItemTooltip->AddToViewport();
	}
	else
	{
		ItemTooltip->UpdateTooltip(ItemData);
		ItemTooltip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	// 마우스 위치 구하기
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	
	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	
	MousePos = MousePos*	ViewportScale;
	
	// 툴팁 크기 구하기
	FVector2D TooltipSize = ItemTooltip->GetDesiredSize();
	
	// 화면 크기 구하기
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	
	// 위치 보정
	if (MousePos.X + TooltipSize.X > ViewportSize.X)
	{
		MousePos.X -= TooltipSize.X;
	}
	if (MousePos.Y + TooltipSize.Y > ViewportSize.Y)
	{
		MousePos.Y -= TooltipSize.Y;
	}
	
		ItemTooltip->SetPositionInViewport(MousePos + FVector2D(20,0));
}

void UUW_InventoryMain::HideItemTooltip()
{
	if (!ItemTooltip) return;
	
	ItemTooltip->SetVisibility(ESlateVisibility::Hidden);
}
