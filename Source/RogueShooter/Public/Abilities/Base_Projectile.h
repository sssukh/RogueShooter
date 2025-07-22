// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Cube;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UParticleSystem> Hit_VFX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	float Damage;
};
