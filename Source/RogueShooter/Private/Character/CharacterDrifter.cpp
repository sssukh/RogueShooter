// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterDrifter.h"


// Sets default values
ACharacterDrifter::ACharacterDrifter()
	:ABase_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterDrifter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterDrifter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterDrifter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

