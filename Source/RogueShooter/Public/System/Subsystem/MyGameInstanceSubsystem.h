// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyGameInstanceSubsystem.generated.h"

enum class EActiveAbilities : uint8;
class UAAbilitySet;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMyGameInstanceSubsystem();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Database")
	TObjectPtr<UAAbilitySet> AAbilityIconDB;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Database")
	TObjectPtr<UPAbilitySet> PAbilityIconDB;

	UFUNCTION()
	TSoftObjectPtr<UTexture2D> GetAAIconFromDB(EActiveAbilities AAbility);

	UFUNCTION()
	TSoftObjectPtr<UTexture2D> GetPAIconFromDB(EPassiveAbilities PAbility);
};
