// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Character.h"
#include "Lobby_Character.generated.h"

class UTextRenderComponent;

UCLASS()
class ROGUESHOOTER_API ALobby_Character : public ABase_Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALobby_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server,Unreliable)
	void S_SetName(const FString& Name);

	UFUNCTION(NetMulticast,Unreliable)
	void MC_SetName(const FString& Name);

	virtual void UpdateClassName_Implementation() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UTextRenderComponent> TextRender;
};
