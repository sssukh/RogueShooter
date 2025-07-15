// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base_Character.h"
#include "Components/SphereComponent.h"
#include "RogueShooter/RSEnumStruct.h"
#include "UObject/Interface.h"
#include "Interface_CharacterManager.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterface_CharacterManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUESHOOTER_API IInterface_CharacterManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	bool IsAlive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void RestoreHealth(float amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	USphereComponent* GetAbilitySphere();

	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	ABase_Character* GetCharacter();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void AdjustPassive(EPassiveAbilities Stat, float MultiplicationAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void Pause(bool Pause, bool Override);

	// TODO : AbilityComponent 구현 필요 
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	// UAbilityComponent* GetAbilityComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void UpdateClassName();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void UpdateCharacterClass(FAvailableCharacter Character);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="RS|Interface|Character Manager")
	void SetupHealthWidget();
};
