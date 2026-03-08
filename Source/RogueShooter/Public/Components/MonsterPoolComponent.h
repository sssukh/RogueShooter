// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterPoolComponent.generated.h"


class ABase_Enemy;

USTRUCT()
struct FEnemyPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ABase_Enemy*> PooledEnemies;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API UMonsterPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMonsterPoolComponent();

	// 풀에서 몬스터를 꺼내오는 함수
	UFUNCTION(BlueprintCallable, Category = "Pool")
	ABase_Enemy* GetMonster(TSubclassOf<ABase_Enemy> MonsterClass, FVector Location, FRotator Rotation);

	// 죽은 몬스터를 풀로 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void ReturnMonster(ABase_Enemy* MonsterToReturn);

protected:
	// 클래스별로 비활성화된 몬스터들을 보관하는 맵 (창고)
	UPROPERTY()
	TMap<TSubclassOf<ABase_Enemy>, FEnemyPoolArray> PoolMap;
};
