// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RogueShooter/RSEnumStruct.h"
#include "FunctionLibrary_Helper.generated.h"

/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UFunctionLibrary_Helper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category = "Function Library Helper",meta = (WorldContext = "WorldContextObject"))
	static void DamageEnemiesOnce(const UObject* WorldContextObject, const TArray<FHitResult>& EnemyHits, float Damage, AController* Instigator, AActor* Causer);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper", BlueprintPure,meta = (WorldContext = "WorldContextObject"))
	static UTexture2D* FindActiveIcon(const UObject* WorldContextObject,EActiveAbilities AAbility=EActiveAbilities::Hammer);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper", BlueprintPure,meta = (WorldContext = "WorldContextObject"))
	static UTexture2D* FindPassiveIcon(const UObject* WorldContextObject,EPassiveAbilities PAbility = EPassiveAbilities::Ability_Bonus_Damage);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper",meta = (WorldContext = "WorldContextObject"))
	static void CreateLoadingScreen(const UObject* WorldContextObject,FText LoadingInfo);

	// TODO : SG_Player 구현 필요 
	// UFUNCTION(BlueprintCallable,Category = "Function Library Helper",meta = (WorldContext = "WorldContextObject"))
	// static SG_Player* LoadPlayerData(const UObject* WorldContextObject);
	
	// TODO : SG_Player 구현 필요 
	// UFUNCTION(BlueprintCallable,Category = "Function Library Helper",meta = (WorldContext = "WorldContextObject"))
	// static void SavePlayerData(const UObject* WorldContextObject,SG_Player* SaveData);
};
