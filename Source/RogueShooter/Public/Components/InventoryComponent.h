// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FInventorySlot;
class UItemData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated); // UI 갱신용 델리게이트

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UPROPERTY(BlueprintAssignable,Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	// 아이템 추가 함수
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemData* NewItem, int32 Amount = 1);
	
	// 아이템 제거 함수
	UFUNCTION(BlueprintCallable)
	bool TakeOutItem(UItemData* TargetItem);
	
	// 인덱스를 이용한 아이템 제거 함수 
	void RemoveItemByIndex(int32 SlotIndex);
	
	// 인벤토리 슬롯에서 아이템 찾기
	UFUNCTION(BlueprintCallable)
	int32 FindItemIndex(const UItemData* TargetItem);

public:
	// 실제 저장소 (DataAsset의 포인터를 저장)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FInventorySlot> InventorySlots;

	// 인벤토리 최대 크기 (예: 20칸)
	UPROPERTY(EditDefaultsOnly)
	int32 Capacity = 20;
};
