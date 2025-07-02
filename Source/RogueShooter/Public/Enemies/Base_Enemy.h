// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base_Enemy.generated.h"

class UInterface_GameManager;
class ASoul;
class USphereComponent;

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


public:
	// Component
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Component")
	TObjectPtr<UParticleSystemComponent> EliteAura;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Component")
	TObjectPtr<USphereComponent> AttackCollisionSphere;

	// Enemy Setup
	
	// TODO : Base_Character 구현 필요
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup", meta = (ExposeOnSpawn = "true"))
	// TArray<ABase_Character*> PlayerArray;

	// 초기값 설정 필요
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<UAnimMontage> AttackAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	float Health = 15.0f;

	// 초기값 설정 필요
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	TObjectPtr<UAnimMontage> DeathAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	bool bIsDead = false;

	// TODO : Base_AIController 구현 필요 
	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base Enemy | Enemy Setup")
	// TObjectPtr<ABase_AIController> BaseControllerReference;

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
};
