// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Pickup.h"
#include "PickupItem.generated.h"

class UItemData;
UCLASS()
class ROGUESHOOTER_API APickupItem : public ABase_Pickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void DoPickupAction(AActor* Character) override;
	
	
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UItemData> ItemReference;
};
