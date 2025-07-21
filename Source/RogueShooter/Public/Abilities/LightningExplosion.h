// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Explosion.h"
#include "LightningExplosion.generated.h"

UCLASS()
class ROGUESHOOTER_API ALightningExplosion : public ABase_Explosion
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightningExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
