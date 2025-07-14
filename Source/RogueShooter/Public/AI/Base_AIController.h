// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/DetourCrowdAIController.h"
#include "Base_AIController.generated.h"

class ABase_Enemy;

UCLASS()
class ROGUESHOOTER_API ABase_AIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_AIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void EndAI();

private:
	UPROPERTY()
	TObjectPtr<ABase_Enemy> AsBaseEnemy;

	// TODO : Base_Character 구현 필요 
	// UPROPERTY()
	// TArray<TObjectPtr<ABase_Character>> PlayerArray;

	UPROPERTY()
	FTimerHandle FindTargetTimerReference;

	UPROPERTY()
	TObjectPtr<AActor> Target;
};
