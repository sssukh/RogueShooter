// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Lobby_Character.h"

#include "Components/TextRenderComponent.h"
#include "System/Lobby/Lobby_PlayerController.h"


// Sets default values
ALobby_Character::ALobby_Character() : Super()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));

	TextRender->SetupAttachment(RootComponent);
	
	TextRender->SetRelativeLocation(FVector(0.0f,0.0f,90.0f));

	
}

// Called when the game starts or when spawned
void ALobby_Character::BeginPlay()
{
	Super::BeginPlay();

	ALobby_PlayerController* LPC = Cast<ALobby_PlayerController>(GetController());

	DisableInput(LPC);

	LoadLastCharacterClass();

}

// Called every frame
void ALobby_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALobby_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALobby_Character::UpdateClassName_Implementation()
{
	Super::UpdateClassName_Implementation();

	S_SetName(Character.CharacterName.ToString());
}

void ALobby_Character::MC_SetName_Implementation(const FString& Name)
{
	TextRender->SetText(FText::FromString(Name));
}

void ALobby_Character::S_SetName_Implementation(const FString& Name)
{
	MC_SetName(Name);
}

