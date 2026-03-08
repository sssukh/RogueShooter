// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MonsterPoolComponent.h"

#include "Enemies/Base_Enemy.h"
#include "System/Base_GameMode.h"


// Sets default values for this component's properties
UMonsterPoolComponent::UMonsterPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ABase_Enemy* UMonsterPoolComponent::GetMonster(TSubclassOf<ABase_Enemy> MonsterClass, FVector Location,
	FRotator Rotation)
{
	if (!MonsterClass) return nullptr;

	// 해당 클래스의 풀(배열)이 존재하는지, 그리고 남은 몬스터가 있는지 확인
	if (PoolMap.Contains(MonsterClass) && PoolMap[MonsterClass].PooledEnemies.Num() > 0)
	{
		// 배열의 맨 마지막 요소를 꺼냄 (가장 빠름)
		ABase_Enemy* PooledMonster = PoolMap[MonsterClass].PooledEnemies.Pop();
        
		// 지정된 위치로 이동 후 활성화
		//PooledMonster->SetActorLocationAndRotation(Location, Rotation);
		// SetActorLocationAndRotation대신 TeleportTo를 사용해서 AI의 네비 시스템이 순간이동한 사실을 알도록 함 
		PooledMonster->TeleportTo(Location,Rotation,false,true);
		PooledMonster->ActivateFromPool(); 
        
		return PooledMonster;
	}

	// 풀이 비어있다면 새로 스폰 (동적 확장)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
	FTransform Transform = FTransform::Identity;
	Transform.SetTranslation(Location);
	Transform.SetRotation(Rotation.Quaternion());
	
	ABase_Enemy* NewMonster = GetWorld()->SpawnActorDeferred<ABase_Enemy>(MonsterClass, Transform);
	if (NewMonster)
	{
		if (ABase_GameMode* GameMode = Cast<ABase_GameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->SetEnemyPlayerArray(NewMonster);
		}
		NewMonster->FinishSpawning(Transform);
	}
	
	
    
	// 새로 스폰된 몬스터도 명시적으로 활성화 처리
	if (NewMonster)
	{
		NewMonster->ActivateFromPool();
	}
    
	return NewMonster;
}

void UMonsterPoolComponent::ReturnMonster(ABase_Enemy* MonsterToReturn)
{
	if (!MonsterToReturn) return;

	// 몬스터 비활성화 처리
	//MonsterToReturn->DeactivateToPool();
	MonsterToReturn->SetActorHiddenInGame(true);
	
	// 해당 클래스의 풀에 다시 집어넣음
	UClass* MonsterClass = MonsterToReturn->GetClass();
	PoolMap.FindOrAdd(MonsterClass).PooledEnemies.Add(MonsterToReturn);
}


