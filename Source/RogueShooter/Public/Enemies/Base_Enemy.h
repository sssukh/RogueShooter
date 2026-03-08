// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Character/RsBaseCharacter.h"
#include "Interface/Interface_CharacterManager.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "Base_Enemy.generated.h"


class UExpSet;
class UCombatSet;
class UHealthSet;
class AFloatingTextActor;
class ABase_AIController;
class UInterface_GameManager;
class ASoul;
class USphereComponent;
class ABase_Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS()
class ROGUESHOOTER_API ABase_Enemy : public ARsBaseCharacter
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
	// TODO : anim_notify로 옮길것 
	UFUNCTION()
	void DamagePlayer();

	void DamageWithGameplayTag();
	
	UFUNCTION(NetMulticast,Unreliable)
	void MC_EnemyAttack();

	UFUNCTION(NetMulticast,Unreliable)
	void MC_OnHit();

	UFUNCTION(NetMulticast,Unreliable)
	void MC_ShowAura();

	// Life and Death
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	
	UFUNCTION(NetMulticast,Unreliable)
	void MC_Enemy_Death();
	
	
	virtual void Die(AActor* DamageCauser) override;
	
	void ApplyXpToTargetPlayer(AActor* TargetPlayer);
	
	void SetTimerWithDelay(float Time, bool bLoop);

	UFUNCTION()
	void ResetDoOnce(); 

	// Spawn XP bubble


	void ShowEliteAura();


	// 왜 필요?
	// Interface
	virtual bool IsAlive_Implementation() override;

	void SetupAbilitiesAndAttributes();
	
	// 피격시(체력감소 시)
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	
	
	// Monster Pool
public:
	void ActivateFromPool();
	void DeactivateToPool();
	// 죽는 애니메이션 재생 후 처리할 풀링 혹은 비주얼 관련 로직 
	virtual void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
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


	// // 초기값 설정 필요
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	// TObjectPtr<UAnimMontage> DeathAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	TSubclassOf<ASoul> SoulClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	bool bIsElite = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	bool ScaleHPToLevel = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UObject> GM_Interface;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	int32 CharLevel = 1;

	// TODO : GAS로 관리하므로 공격 관련 변수와 로직 삭제
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
	// 여기까지 삭제 
	
	FAudioDeviceHandle EnemySoundHandle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy")
	TObjectPtr<USoundBase> EnemySound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy")
	TObjectPtr<USoundBase> ImpactSound;
	
	FDoOnce TakeDamageDoOnce;
	
	
	void AddCharacterAbilities();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// 부여할 어빌리티 목록
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category =  "GAS | Config")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category =  "GAS | Config")
	TSubclassOf<UGameplayEffect> DropExpClass;
	
	// Delegate
	// GameManager에서만 관리중이므로 일단 두고 나중에 GameManager 정리할 때 치우자 
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Base Enemy|Delegate")
	FOnDeath OnDeath;
};
