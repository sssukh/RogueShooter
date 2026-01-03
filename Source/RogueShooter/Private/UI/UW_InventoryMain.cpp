// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InventoryMain.h"

#include "Blueprint/SlateBlueprintLibrary.h"
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
	
	ItemTooltip->AddToViewport(1);
	
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

void UUW_InventoryMain::ShowItemTooltip(const UItemData* ItemData, const int32 ItemCount)
{
	if (!ItemData) return;
	
	// 위젯이 존재하지 않는다면 생성 및 viewport에 추가
	if (!ItemTooltip)
	{
		ItemTooltip = CreateWidget<UUW_ItemTooltip>(this,ItemTooltipClass);
		
		ItemTooltip->UpdateTooltip(ItemData, ItemCount);
		
		ItemTooltip->AddToViewport(1);
	}
	else
	{
		ItemTooltip->UpdateTooltip(ItemData, ItemCount);
		ItemTooltip->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	UpdateMousePosition();		
}

void UUW_InventoryMain::HideItemTooltip()
{
	if (!ItemTooltip) return;
	
	ItemTooltip->SetVisibility(ESlateVisibility::Hidden);
}

void UUW_InventoryMain::UpdateMousePosition()
{
	if (!ItemTooltip || !ItemTooltip->IsVisible()) return;
	
	// 마우스의 절대 좌표 가져오기
	// GetMousePositionOnViewport와 GetMousePosition은 다르다.
	APlayerController* PC = GetOwningPlayer();		
	if (!PC) return;
	
	// DPI scale 값 가져오기
	float DPI_Scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	
	// 마우스 물리 좌표(픽셀)
	float MouseX,MouseY;
	if (!PC->GetMousePosition(MouseX,MouseY)) return;
	FVector2D MousePixelPos(MouseX,MouseY);
	
	// 마우스를 '논리 좌표'로 변환
	// 공식 : 논리좌표 = 물리좌표/Scale
	FVector2D LogicalMousePos = FVector2D::ZeroVector;
	if (DPI_Scale>0.0f)
	{
		LogicalMousePos = MousePixelPos / DPI_Scale;
	}
	
	
	// 논리단위
	
	// 툴팁의 현재 크기
	FVector2D LogicalTooltipSize = ItemTooltip->GetDesiredSize();
	
	FVector2D LogicalViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	
	FVector2D TargetPos = LogicalMousePos + mMouseOffset;
	
	// X축 보정
	if (TargetPos.X + LogicalTooltipSize.X > LogicalViewportSize.X)
	{
		TargetPos.X -= LogicalTooltipSize.X;
	}
	
	// Y축 보정
	if (TargetPos.Y + LogicalTooltipSize.Y > LogicalViewportSize.Y)
	{
		TargetPos.Y -= LogicalTooltipSize.Y;
	}
	
	// RenderTranslation으로 위치이동
	// SetPositionInViewport대신 쓰면 레이아웃 재계산을 안해서 훨씬 가볍다.
	// ItemTooltip->SetRenderTranslation(TargetPos);
	ItemTooltip->SetPositionInViewport(TargetPos,false);
}
