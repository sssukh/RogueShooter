// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Data/Item/ItemData.h"
#include "RogueShooter/RSEnumStruct.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(UItemData* NewItem, int32 Amount)
{
	if (InventorySlots.Num() >= Capacity || !NewItem) return false;

	int32 ItemIndex = FindItemIndex(NewItem);
	
	
	// 인벤토리에 존재하지 않으므로 새로운 아이템 슬롯을 추가.
	if (ItemIndex == INDEX_NONE)
	{
		InventorySlots.Add(NewItem);
		
	}
	
	// 인벤토리에 존재하므로 개수를 1 증가.
	else
	{
		InventorySlots[ItemIndex].Count++;
		
	}
	
	// 아이템이 들어왔으니 UI에게 갱신하라고 알림
	OnInventoryUpdated.Broadcast(); 
	
	return true;
}

bool UInventoryComponent::TakeOutItem(UItemData* TargetItem)
{
	if (!TargetItem || InventorySlots.IsEmpty()) return false;
	
	int32 ItemIndex = FindItemIndex(TargetItem);
	
	if (ItemIndex == INDEX_NONE) return false;
	
	RemoveItemByIndex(ItemIndex);
	
	return true;
}

void UInventoryComponent::RemoveItemByIndex(int32 SlotIndex)
{
	FInventorySlot& Slot = InventorySlots[SlotIndex];

	Slot.Count -=1;
	
	if (Slot.Count<=0)
	{
		// 슬롯 비우기
		InventorySlots.RemoveAt(SlotIndex);
	}
	
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::FindItemIndex(const UItemData* TargetItem)
{
	if (!TargetItem || InventorySlots.IsEmpty()) return INDEX_NONE;
	
	// 아이템이 같고 개수가 1개 이상인 경우를 찾기.
	// 없을 시 INDEX_NONE 반환
	return InventorySlots.IndexOfByPredicate([TargetItem](const FInventorySlot& Slot)
	{
		return (Slot.ItemData == TargetItem) && (Slot.Count>0);
	});
		
}

