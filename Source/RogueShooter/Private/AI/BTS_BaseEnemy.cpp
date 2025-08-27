// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS_BaseEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "Enemies/Base_Enemy.h"
#include "AI/Base_AIController.h"

UBTS_BaseEnemy::UBTS_BaseEnemy()
{
}

void UBTS_BaseEnemy::SetBehavior(UBehaviorTreeComponent& OwnerComp, EEnemyState Value)
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		BlackboardComp->SetValueAsEnum(Behavior.SelectedKeyName, (uint8)Value);
	}
}

void UBTS_BaseEnemy::Update(UBehaviorTreeComponent& OwnerComp)
{
	if(IsValid(BaseEnemy) && IsValid(Base_AIController))
	{
		if(UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent())
		{
			TargetReference = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName));
		}

		if(IsValid(TargetReference))
		{
			SetBehavior(OwnerComp,EEnemyState::Active);
		}
		else
		{
			SetBehavior(OwnerComp,EEnemyState::Disabled);

			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([&]()
			{
				Update(OwnerComp);
			}),1.0f,false);
		}
	}
}



void UBTS_BaseEnemy::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	BaseEnemy = Cast<ABase_Enemy>(OwnerComp.GetAIOwner()->GetPawn());

	Base_AIController = Cast<ABase_AIController>(OwnerComp.GetAIOwner());

	Update(OwnerComp);	
}
