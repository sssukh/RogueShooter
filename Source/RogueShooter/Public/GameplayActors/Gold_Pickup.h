// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Pickup.h"
#include "Gold_Pickup.generated.h"

UCLASS()
class ROGUESHOOTER_API AGold_Pickup : public ABase_Pickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGold_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void DoPickupAction(AActor* Character) override;

	virtual void MC_PickupEffects() override;
	
public:
	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> P_DeadMans_Loot;

	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 GoldAmount = 5;
};
