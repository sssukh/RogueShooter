// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "Data/Item/ItemData.h"
#include "RogueShooter/RSEnumStruct.h"
#include "AbilitySystemBlueprintLibrary.h"


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
		InventorySlots.Add(FInventorySlot(NewItem));
		ItemIndex = InventorySlots.Num()-1;
	}
	
	// 인벤토리에 존재하므로 개수를 1 증가.
	else
	{
		InventorySlots[ItemIndex].Count++;
	}
	
	// 아이템의 개수가 변했으므로 효과 갱신
	UpdateSlotEffect(ItemIndex);
	
	// 아이템의 ability 갱신
	UpdateSlotAbility(ItemIndex);
	
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
	
	// 아이템의 개수가 변했으므로 효과 갱신
	UpdateSlotEffect(SlotIndex);
	
	UpdateSlotAbility(SlotIndex);
	
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

void UInventoryComponent::UpdateSlotEffect(int32 slotIndex)
{
	if (!InventorySlots.IsValidIndex(slotIndex)) return;

	FInventorySlot& Slot = InventorySlots[slotIndex];
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
    
	if (!ASC) return;

	// 1. 기존 효과가 있다면 제거 (Reset)
	if (Slot.ActivePassiveEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(Slot.ActivePassiveEffectHandle);
		Slot.ActivePassiveEffectHandle.Invalidate(); // 핸들 초기화
	}

	// 2. 아이템이 없거나 개수가 0이면 종료 (효과 제거만 하고 끝)
	if (Slot.IsEmpty() || !Slot.ItemData->PassiveEffectClass)
	{
		return;
	}

	// 3. 새로운 수치 계산 (기본 스탯 * 개수)
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Slot.ItemData->PassiveEffectClass, 1.0f, Context);

	if (SpecHandle.IsValid())
	{
		for (const auto& Pair : Slot.ItemData->ItemStats)
		{
			FGameplayTag Tag = Pair.Key;       // 예: Stat.Attack
			float BaseValue = Pair.Value;      // 예: 10.0
            
			// [핵심 로직] 개수만큼 곱하기
			float TotalValue = BaseValue * (float)Slot.Count; 

			// 곱해진 최종 값을 SetByCaller로 주입
			SpecHandle.Data->SetSetByCallerMagnitude(Tag, TotalValue);
		}

		// 4. 효과 적용 및 새 핸들 저장
		Slot.ActivePassiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UInventoryComponent::UpdateSlotAbility(int32 slotIndex)
{
	FInventorySlot& Slot = InventorySlots[slotIndex];
	UAbilitySystemComponent* ASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	if (!ASC) return;

	// 1. 아이템이 없거나, 능력이 없는 아이템이라면 -> 기존 능력 제거
	if (Slot.IsEmpty() || !Slot.ItemData->PassiveAbilityClass)
	{
		if (Slot.ActiveAbilitySpecHandle.IsValid())
		{
			ASC->ClearAbility(Slot.ActiveAbilitySpecHandle);
			Slot.ActiveAbilitySpecHandle = FGameplayAbilitySpecHandle();
		}
		return;
	}

	// 2. 이미 능력을 가지고 있다면? -> 레벨(스택)만 업데이트
	if (Slot.ActiveAbilitySpecHandle.IsValid())
	{
		// FindAbilitySpecFromHandle로 명세서를 찾아서 레벨 수정
		if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Slot.ActiveAbilitySpecHandle))
		{
			Spec->Level = Slot.Count; // 개수 = 레벨
			// 패시브라 레벨 변경 후 다시 활성화가 필요할 수도 있고, 
			// 내부 로직에서 GetAbilityLevel()을 쓸 것이므로 값만 바꿔도 됨.
		}
	}
	// 3. 능력이 없다면? -> 새로 부여 (Give Ability)
	else
	{
		// 레벨을 아이템 개수(Count)로 설정하여 부여
		FGameplayAbilitySpec NewSpec(Slot.ItemData->PassiveAbilityClass, Slot.Count);
        
		// 아이템 장착 시 바로 발동되어야 하므로 GiveAbilityAndActivate 사용 고려
		// (보통 패시브는 NetExecutionPolicy에 따라 자동 활성화됨)
		Slot.ActiveAbilitySpecHandle = ASC->GiveAbility(NewSpec);
        
		// 패시브라면 즉시 활성화 시도
		ASC->TryActivateAbility(Slot.ActiveAbilitySpecHandle);
	}
}


