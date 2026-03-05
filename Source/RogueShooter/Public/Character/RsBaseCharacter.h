// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/Interface_CharacterManager.h"
#include "RsBaseCharacter.generated.h"

class UCombatSet;
class UHealthSet;

UCLASS()
class ROGUESHOOTER_API ARsBaseCharacter : public ACharacter, public IInterface_CharacterManager, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARsBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Die(AActor* DamageCauser);
	
	virtual void ApplyAttributeOnLevel(float NewLevel);
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GAS|Config")
	TSubclassOf<UGameplayEffect> DefaultCurveEffectClass;
	
	UPROPERTY()
	TObjectPtr<UHealthSet> HealthSet;
	
	UPROPERTY()
	TObjectPtr<UCombatSet> CombatSet;
	
};
