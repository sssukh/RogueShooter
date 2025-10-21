// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIAssetCacheSubsystem.generated.h"

struct FStreamableManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetLoaded, UObject*, LoadedAsset);
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UUIAssetCacheSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Asset Cache Config")
	int32 MaxCacheSize = 100;

	UFUNCTION(Category="Asset Cache")
	void RequestAsset(TSoftObjectPtr<UObject> AssetToLoad, const FOnAssetLoaded& OnLoaded);

private:
	// 1. 실제 캐시
	UPROPERTY()
	TMap<TSoftObjectPtr<UObject>,UObject*> LoadedAssetCache;

	// 2. 사용 순서를 추적하는 배열
	TArray<TSoftObjectPtr<UObject>> UsageOrder;

	// 3. 로드 중인 애셋의 콜백 목록 
	TMap<TSoftObjectPtr<UObject>,TArray<FOnAssetLoaded>> PendingLoadCallbacks;

	TSharedPtr<FStreamableManager> StreamableManager;

	// 애셋 사용 갱신
	void UpdateUsageOrder(const TSoftObjectPtr<UObject>& Asset);

	// 가장 오래된 애셋을 캐시에서 제거
	void EvictLRUItem();
};
