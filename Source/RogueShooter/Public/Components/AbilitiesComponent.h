// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "AbilitiesComponent.generated.h"


class AFireball_Projectile;
class ALightningExplosion;
class ABase_Projectile;
class ABase_Character;
enum class EPassiveAbilities : uint8;
enum class EActiveAbilities : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API UAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilitiesComponent(const FObjectInitializer& ObjectInitializer);

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
	void S_ExecuteHammer(const TArray<FHitResult>& Hits,float Damage, float Radius, APlayerController* Controller);

	UFUNCTION(NetMulticast,Reliable)
	void MC_Hammer(float Radius);

	// FrostBolt
	void LevelUpFrostBolt();

	void GrantFrostBolt(bool Cast);

	UFUNCTION()
	void PrepareFrostBolt();
	
	UFUNCTION(Server,Reliable)
	void S_ExecuteFrostBolt(AActor* Target, ABase_Character* Character, float Damage);

	UFUNCTION(NetMulticast,Reliable)
	void MC_Frostbolt();

	// Lightning
	void LevelUpLightning();

	void GrantLightning(bool Cast);

	UFUNCTION()
	void PrepareLightning();
	
	UFUNCTION(Server,Reliable)
	void S_ExecuteLightning(const FVector& TargetLocation, ABase_Character* Instigator, float Damage, float Radius);

	// Fireball
	void LevelUpFireball();

	void GrantFireball(bool Cast);

	UFUNCTION()
	void PrepareFireball();
	
	UFUNCTION(Server,Reliable)
	void S_ExecuteFireball(AActor* Target, ABase_Character* Character, float Damage, float Radius);

	UFUNCTION(NetMulticast,Reliable)
	void MC_Fireball();

	// Passive Ability
public:
	void LevelUpMaxHealth(bool PowerUp);

	void LevelUpTimerReduction(bool PowerUp);

	void LevelUpAbilityDamageBonus(bool PowerUp);

	void LevelUpSpeedBonus(bool PowerUp);

	
	// Check if this character already has this ability - if not - add it into the ability map
	// 캐릭터가 이 능력을 이미 소유했는지 확인하고 그렇지 않으면 ability map에 추가합니다.
	int32 LevelUpActive(EActiveAbilities Active);

	int32 LevelUpPassive(EPassiveAbilities Passive);

	
	void RefreshAbilities();

	void InvalidateTimers();

	// SG_Player 구현 필요 
	void SetStartingAbility();
	
	UFUNCTION()
	float CalcAbilityDamageWithCrit(float weight, EActiveAbilities ActiveAbility,float BaseDamage);
	
public:
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
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitiesComponent | Setup")
	// EDrawDebugTrace::Type Debug;
	
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

	TSubclassOf<ABase_Projectile> BaseProjectileClass;

	TSubclassOf<ALightningExplosion> LightningClass;

	TSubclassOf<AFireball_Projectile> FireballClass;

	FFlipflop FireballFlipflop;

	
};
