// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "Base_Enemy.generated.h"



class AFloatingTextActor;
class ABase_AIController;
class UInterface_GameManager;
class ASoul;
class USphereComponent;
class ABase_Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class ROGUESHOOTER_API ABase_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Combat
	UFUNCTION(Category = "On Overlap Event")
	void AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "On Overlap Event")
	void AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// TODO : DO Once를 사용했는데 실질적으로 반복해서 리셋하고 사용한다. 수정 필요 
	UFUNCTION()
	void DamagePlayer();

	// TODO : Multicast이다. 어떻게 적용해야할까?
	UFUNCTION()
	void MC_EnemyAttack();

	UFUNCTION()
	void MC_OnHit();

	UFUNCTION()
	void MC_ShowAura();

	// Life and Death
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	void MC_Enemy_Death();
	
	void SetTimerWithDelay(float Time, bool bLoop);

	UFUNCTION()
	void ResetDoOnce(); 

	// Floating Combat Text
	
	UFUNCTION()
	void SpawnFloatingText(float InDamage);

	// Spawn XP bubble

	UFUNCTION()
	void SpawnSoul();
	
public:
	// Component
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Component")
	TObjectPtr<UParticleSystemComponent> EliteAura;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Component")
	TObjectPtr<USphereComponent> AttackCollisionSphere;

	// Enemy Setup
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	TArray<TObjectPtr<ABase_Character>> PlayerArray;

	// 초기값 설정 필요
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<UAnimMontage> AttackAnimation;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	float Damage = 10.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	float Health = 15.0f;

	// 초기값 설정 필요
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<UAnimMontage> DeathAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<ABase_AIController> BaseControllerReference;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	TSubclassOf<ASoul> Soul;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	bool bIsElite = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	bool ScaleHPToLevel = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UObject> GM_Interface;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	int32 CharLevel = 1;

	// Attack Logic
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<AActor> PlayerToDamage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	FTimerHandle DamagePlayerTimerReference;
	
	FTimerDelegate DamageSphereOverlapDelegate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	FTimerHandle RetriggerHandle;

	FTimerDelegate RetriggerDelegate;
	
	FDoOnce DoOnce;

	FAudioDeviceHandle EnemySoundHandle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy")
	TObjectPtr<USoundBase> EnemySound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy")
	TObjectPtr<USoundBase> ImpactSound;
	
	FDoOnce TakeDamageDoOnce;

	UPROPERTY()
	TSubclassOf<ASoul> SoulClass;

	UPROPERTY()
	TSubclassOf<AFloatingTextActor> FTActorClass;
	
	// Delegate
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Enemy|Delegate")
	FOnDeath OnDeath;
};
