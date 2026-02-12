// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RsBaseCharacter.h"


// Sets default values
ARsBaseCharacter::ARsBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARsBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARsBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARsBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ARsBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ARsBaseCharacter::Die(AActor* DamageCauser)
{
}

UAbilitySystemComponent* ARsBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

