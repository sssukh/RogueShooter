// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InventoryMain.h"

#include "Components/InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "RogueShooter/AssetPath.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UI/UW_ItemIcon.h"


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
}

void UUW_InventoryMain::RefreshInventory()
{
	if (!GridPanel || !PlayerInventoryComponent.IsValid()) return;
	
	GridPanel->ClearChildren();
	
	for (int32 i = 0; i < PlayerInventoryComponent->InventorySlots.Num(); ++i)
	{
		UUW_ItemIcon* NewSlot = CreateWidget<UUW_ItemIcon>(this,ItemIconClass);
		
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
