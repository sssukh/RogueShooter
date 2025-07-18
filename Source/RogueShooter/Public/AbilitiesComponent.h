// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitiesComponent.generated.h"


enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilitiesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	float CalculateTimerMode(float InMultiplyer);

	float  CalculateBonusDamage(float InMultiplyer);

	bool CheckEvoActive(EActiveAbilities InEnumActive);

	// Active Ability

public:
	// Hammer
	void LevelUpHammer();

	void GrantHammer(bool Cast);

	UFUNCTION()
	void PrepareHammer();

	UFUNCTION(Server,Reliable)
	void S_ExecuteHammer(TArray<FHitResult> Hits,float Damage, float Radius, APlayerController* Controller);

	UFUNCTION(NetMulticast,Reliable)
	void MC_Hammer(float Radius);

	UFUNCTION()
	float CalcAbilityDamageWithCrit(float weight, EActiveAbilities ActiveAbility,float BaseDamage);
	
private:
	// Ability
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Passive")
	float AbilityTimerMultiplier = 1.0f;
	
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Passive")
	float AbilityDamageMultiplier = 1.0f;

	// Actives
	UPROPERTY(Replicated,VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float HammerTimer = 1.0f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float HammerDamage = 8.0f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float HammerRadius = 300.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FBTimer = 1.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FBDamage = 5.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float LightningTimer = 2.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float LightningDamage = 20.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float LightningRadius = 150.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	int32 FBFireCount = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FBFireIndex = 0.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FireballDamage = 10.0f;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FireballTimer = 2.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Actives")
	float FireballRadius = 150.0f;

	// Abilities
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TMap<EActiveAbilities,int32> ActiveAbilitiesMap;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TMap<EActiveAbilities,EPassiveAbilities> EvolutionMap;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TMap<EPassiveAbilities,int32> PassiveAbilitiesMap;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TArray<EPassiveAbilities> EvolutionPassiveArray;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	int32 MaxAbilityLevel = 5;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TArray<bool> EvolutionTracker;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	TArray<FTimerHandle> ActiveTimers;

	// Setup
	// Kismet 사용. 어떻게 할까..
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Setup")
	EDrawDebugTrace::Type Debug;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Gameplay")
	TObjectPtr<AActor> NearestActor;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent")
	FVector LastLightningLoc;

	// Active Ability Delegate
	FTimerDelegate HammerDelegate;
	FTimerDelegate FireBallDelegate;
	FTimerDelegate FrostBoltDelegate;
	FTimerDelegate LightningDelegate;

	// Emitter
	UPROPERTY()
	TObjectPtr<UParticleSystem> HammerEffect;
	
	UPROPERTY()
	TObjectPtr<USoundBase> HammerSound;

	UPROPERTY()
	TObjectPtr<UParticleSystem> FireballEffect;

	UPROPERTY()
	TObjectPtr<USoundBase> FireballSound;

	UPROPERTY()
	TObjectPtr<UParticleSystem> FrostBoltEffect;

	UPROPERTY()
	TObjectPtr<USoundBase> FrostBoltSound;

	UPROPERTY()
	TObjectPtr<UParticleSystem> LightningEffect;

	UPROPERTY()
	TObjectPtr<USoundBase> LightningSound;
};
