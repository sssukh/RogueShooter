// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Enemy.h"
#include "Base_Elite.generated.h"

UCLASS()
class ROGUESHOOTER_API ABase_Elite : public ABase_Enemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Elite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
