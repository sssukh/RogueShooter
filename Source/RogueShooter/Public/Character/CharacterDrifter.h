// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Character.h"
#include "CharacterDrifter.generated.h"

class UCrosshairAttSet;

UCLASS()
class ROGUESHOOTER_API ACharacterDrifter : public ABase_Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterDrifter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
