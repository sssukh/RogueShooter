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
	UFUNCTION(BlueprintCallable,Category = "Function Library Helper")
	void DamageEnemiesOnce(const TArray<FHitResult>& EnemyHits, float Damage, AController* Instigator, AActor* Causer, const TSubclassOf<UDamageType> DamageTypeClass);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper", BlueprintPure)
	UTexture2D* FindActiveIcon(EActiveAbilities AAbility=EActiveAbilities::Hammer);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper", BlueprintPure)
	UTexture2D* FindPassiveIcon(EPassiveAbilities PAbility = EPassiveAbilities::Ability_Bonus_Damage);

	UFUNCTION(BlueprintCallable,Category = "Function Library Helper")
	void CreateLoadingScreen(FText LoadingInfo);

	// TODO : SG_Player 구현 필요 
	// UFUNCTION(BlueprintCallable,Category = "Function Library Helper")
	// SG_Player* LoadPlayerData();
	
	// TODO : SG_Player 구현 필요 
	// UFUNCTION(BlueprintCallable,Category = "Function Library Helper")
	// void SavePlayerData(SG_Player* SaveData);
};
