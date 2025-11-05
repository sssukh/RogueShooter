// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Base_AIController.h"

#include "Character/Base_Character.h"
#include "Enemies/Base_Enemy.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Interface_CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Utility/RSLog.h"


// Sets default values
ABase_AIController::ABase_AIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ABase_AIController::BeginPlay()
{
	Super::BeginPlay();

	if(BehaviorTree==nullptr)
	{
		BehaviorTree = Cast<UBehaviorTree>(
			StaticLoadObject(UBehaviorTree::StaticClass(), nullptr, *AssetPath::AI::BaseEnemyBT)
		);
		
	}
}

// Called every frame
void ABase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ABase_Enemy* Enemy = Cast<ABase_Enemy>(InPawn))
	{
		AsBaseEnemy = Enemy;
		
		RS_LOG_ERROR(TEXT("%s has been set to %s"),*this->GetName(), *Enemy->GetName())

		PlayerArray = AsBaseEnemy->PlayerArray;

		if(BehaviorTree == nullptr)
		{
			RS_LOG_ERROR(TEXT("BehaviorTree has not been set"));
		}
		
		RunBehaviorTree(BehaviorTree);

		BeginAI();
	}
}

void ABase_AIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// RS_LOG_ERROR(TEXT("%s has called EndPlay"),*this->GetName())
}

void ABase_AIController::OnUnPossess()
{
	Super::OnUnPossess();

	// RS_LOG_ERROR(TEXT("%s has called OnUnPossess"),*this->GetName())
}

void ABase_AIController::BeginAI()
{
	FindTarget();

	GetWorldTimerManager().SetTimer(FindTargetTimerReference,FTimerDelegate::CreateLambda([this]()
	{
		FindTarget();
	}),
	1.0f,
	true);
}

void ABase_AIController::FindTarget()
{
	if(IsValid(Target))
	{
		if(Target->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			bool bAlive = IInterface_CharacterManager::Execute_IsAlive(Target);
			
			if(!bAlive)
			{
				UpdateTargetArray();
			}
		}
	}

	if(!IsValid(Blackboard))
	{
		RS_LOG_ERROR(TEXT("Blackboard Failed to Load"));
	}

	float Distance;

	TArray<AActor*> ActorArray;

	for(ABase_Character* bc : PlayerArray)
	{
		ActorArray.Add(bc);
	}
	
	AActor* NearestActor = UGameplayStatics::FindNearestActor(GetPawn()->GetActorLocation(),ActorArray,Distance);
	
	Blackboard->SetValueAsObject(FName("Target"),NearestActor);

	Target = NearestActor;
}

void ABase_AIController::EndAI()
{
	GetWorldTimerManager().ClearTimer(FindTargetTimerReference);

	if(IsValid(Blackboard))
	{
		// ai의 움직임을 멈추기 위해 blackboard의 target 값을 비운다.
		Blackboard->SetValueAsObject(FName("Target"),nullptr);
	}
}

void ABase_AIController::UpdateTargetArray()
{
	StopMovement();

	PlayerArray.Empty();

	TArray<APlayerState*> WorldPlayerStateArray = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
	
	for(APlayerState* ps : WorldPlayerStateArray)
	{
		APawn* PlayingPawn = ps->GetPawn();

		if(PlayingPawn->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
		{
			if(IInterface_CharacterManager::Execute_IsAlive(PlayingPawn))
			{
				PlayerArray.Add(IInterface_CharacterManager::Execute_GetCharacter(PlayingPawn));
			}
		}
	}
}

