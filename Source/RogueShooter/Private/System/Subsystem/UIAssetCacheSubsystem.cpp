// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Subsystem/UIAssetCacheSubsystem.h"

#include "Engine/AssetManager.h"
#include "Utility/RSLog.h"

void UUIAssetCacheSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void UUIAssetCacheSubsystem::Deinitialize()
{
	Super::Deinitialize();

	LoadedAssetCache.Empty();

	UsageOrder.Empty();

	PendingLoadCallbacks.Empty();
}

void UUIAssetCacheSubsystem::RequestAsset(TSoftObjectPtr<UObject> AssetToLoad, const FOnAssetLoaded& OnLoaded)
{
	if(AssetToLoad==nullptr)
	{
		RS_LOG_WARNING(TEXT("Asset To Load is Null"))
		return;
	}

	// 캐시 히트
	if(UObject* FoundAsset = LoadedAssetCache.FindRef(AssetToLoad))
	{
		UpdateUsageOrder(AssetToLoad);

		if(OnLoaded.IsBound())
			OnLoaded.Broadcast(FoundAsset);
		return;
	}

	// 펜딩 히트
	if(TArray<FOnAssetLoaded>* Callbacks = PendingLoadCallbacks.Find(AssetToLoad))
	{
		Callbacks->Add(OnLoaded);
		return;
	}

	// 캐시 미스
	// 펜딩 목록에 추가 
	PendingLoadCallbacks.Add(AssetToLoad,{OnLoaded});

	// 비동기 로드 완료 시 호출될 람다.
	auto OnLoadCompletedLambda = [this,AssetToLoad]()
	{
		UObject* LoadedAsset = AssetToLoad.Get();

		if(LoadedAsset)
		{
			// 캐시에 애셋 추가 전에 캐시가 가득 찼는지 확인.
			if(LoadedAssetCache.Num()>MaxCacheSize)
			{
				EvictLRUItem();
			}

			// 새 애셋을 캐시에 추가
			LoadedAssetCache.Add(AssetToLoad,LoadedAsset);

			// 캐시의 순서목록 갱신
			UpdateUsageOrder(AssetToLoad);
		}

		// 펜딩 목록에서 탐색 및 제거
		TArray<FOnAssetLoaded> Callbacks = PendingLoadCallbacks.FindAndRemoveChecked(AssetToLoad);

		for(const FOnAssetLoaded& Callback : Callbacks)
		{
			if(Callback.IsBound())
				Callback.Broadcast(LoadedAsset);
		}
	};

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(OnLoadCompletedLambda));
}

void UUIAssetCacheSubsystem::UpdateUsageOrder(const TSoftObjectPtr<UObject>& Asset)
{
	// 순서 목록에서 제거
	UsageOrder.Remove(Asset);

	// 순서 목록 맨 뒤에 추가해서 최근 사용한 항목으로 최신화
	UsageOrder.Add(Asset);
}

void UUIAssetCacheSubsystem::EvictLRUItem()
{
	if(UsageOrder.IsEmpty())
		return;

	// 가장 오래된 애셋 탐색
	TSoftObjectPtr<UObject> LRUItem = UsageOrder[0];

	// 순서 목록에서 가장 오래된 애셋 제거 
	UsageOrder.RemoveAt(0);

	// 애셋 목록에서 가장 오래된 애셋 제거 
	LoadedAssetCache.Remove(LRUItem);

	RS_LOG_WARNING(TEXT("LRU Cache Evicted %s"),*LRUItem->GetName());
}
