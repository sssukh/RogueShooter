// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base_Projectile.h"
#include "Fireball_Projectile.generated.h"

class ABase_Explosion;

UCLASS()
class ROGUESHOOTER_API AFireball_Projectile : public ABase_Projectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireball_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	float Radius;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	TSubclassOf<ABase_Explosion> ExplosionClass;
	
};
