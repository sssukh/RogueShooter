// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_BaseEnemy.generated.h"

enum class EEnemyState : uint8;
class ABase_AIController;
class ABase_Enemy;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UBTS_BaseEnemy : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_BaseEnemy();

	void SetBehavior(UBehaviorTreeComponent& OwnerComp, EEnemyState Value);

	// Continue chasing until target is invalid, then wait and retry every 1 second
	void Update(UBehaviorTreeComponent& OwnerComp);
protected:
	// 서비스가 활성화될 때 호출
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// // 서비스가 비활성화될 때 호출
	// virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//
	// // 서비스 주기적 Tick 함수 (DeltaSeconds: 지난 시간)
	// virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Default")
	TObjectPtr<ABase_Enemy> BaseEnemy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Default")
	TObjectPtr<ABase_AIController> Base_AIController;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Default")
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Default")
	FBlackboardKeySelector Behavior;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Default")
	TObjectPtr<AActor> TargetReference;
};
