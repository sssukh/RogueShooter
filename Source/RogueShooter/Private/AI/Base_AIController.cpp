// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Base_AIController.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


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
	
}

// Called every frame
void ABase_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_AIController::EndAI()
{
	GetWorldTimerManager().ClearTimer(FindTargetTimerReference);

	if(Blackboard != nullptr)
	{
		// ai의 움직임을 멈추기 위해 blackboard의 target 값을 비운다.
		// TODO: blackboard 설정 
		// Blackboard->SetValue<FName("Target")>;
	}
}

