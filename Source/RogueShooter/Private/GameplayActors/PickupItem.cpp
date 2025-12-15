// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/PickupItem.h"

#include "Components/InventoryComponent.h"
#include "Data/Item/ItemData.h"


// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemReference && ItemReference->ItemMesh)
		StaticMesh->SetStaticMesh(ItemReference->ItemMesh);
}



// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupItem::DoPickupAction(AActor* Character)
{
	Super::DoPickupAction(Character);
	
	// 캐릭터의 인벤토리 컴포넌트를 찾아서 추가
	if (UInventoryComponent* Inven = Character->FindComponentByClass<UInventoryComponent>())
	{
		if(Inven->AddItem(ItemReference))
		{
			Destroy(); // 획득 성공 시 월드에서 제거
		}
	}
}

