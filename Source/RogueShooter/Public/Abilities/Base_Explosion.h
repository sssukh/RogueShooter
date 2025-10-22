// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Base_Explosion.generated.h"

UCLASS()
class ROGUESHOOTER_API ABase_Explosion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Explosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast,Unreliable)
	void MC_VFX();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	float Radius = 100.0f;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	// EDrawDebugTrace::Type DrawDebugType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UParticleSystem> DamageVFX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (ExposeOnSpawn = "true"))
	TObjectPtr<USoundBase> SoundVFX;
};
