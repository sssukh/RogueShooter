// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Base_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ROGUESHOOTER_API ABase_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
public:
	// 충돌 컴포넌트 
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	// 메쉬(교체 필요)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ProjectileStaticMesh;

	// 이동 컴포넌트
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UParticleSystem> Hit_VFX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	float Damage;
	
	// 대미지 명세서
	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
};
