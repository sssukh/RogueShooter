// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/DetourCrowdAIController.h"
#include "Base_AIController.generated.h"

class ABase_Character;
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

public:
	// On Possess setup references and begin BT
	// 빙의하면서 reference 셋업(Enemy에 reference 전달)을 하고 BT를 시작시킨다.
	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnUnPossess() override;
	
	// when AI starts - find a good player to chase
	// AI가 동작하기 시작하면 추적할 플레이어를 찾는다.
	UFUNCTION()
	void BeginAI();

	
	UFUNCTION()
	void FindTarget();

	// stop AI on death
	// 죽으면 AI를 멈춘다.
	UFUNCTION()
	void EndAI();

	// find a player that is alive
	// 살아있는 player를 찾는다.
	UFUNCTION()
	void UpdateTargetArray();

private:
	UPROPERTY()
	TObjectPtr<ABase_Enemy> AsBaseEnemy;

	UPROPERTY()
	TArray<TObjectPtr<ABase_Character>> PlayerArray;

	UPROPERTY()
	FTimerHandle FindTargetTimerReference;

	UPROPERTY()
	TObjectPtr<AActor> Target;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
