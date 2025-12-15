// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
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
};
