// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 아이템의 정보를 담는 데이터 클래스
 * GameplayTag를 이용해 캐릭터의 스탯에 영향을 준다.
 */
UCLASS()
class ROGUESHOOTER_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Item")
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Item")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Item")
	FText Description; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Stack")
	bool bIsStackable = true; // 겹치기 가능 여부
	
	// 이후에 최적화를 위해 soft 참조로 변경 필요 
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= "Item")
	TObjectPtr<UStaticMesh> ItemMesh;
	
	// 예: { "Stat.Attack" : 10.0 }
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TMap<FGameplayTag, float> ItemStats;
	
	// 획득 시 스탯에 적용할 효과 
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> PassiveEffectClass;
	
	// 로직용 패시브 능력 (적 처치 시 구슬 생성)
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayAbility> PassiveAbilityClass;
};
