// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stash.generated.h"

class ABase_Pickup;
class USphereComponent;

UCLASS()
class ROGUESHOOTER_API AStash : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn Item when stash takes damage
	void SpawnItem();

	void ResetActivation();

	void ResetStash();

	UFUNCTION(NetMulticast,Unreliable)
	void HideStash(bool Hide);

	bool CheckForItems();

	// If Stash is damaged - give item and hide actor until reset time hits
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	UPROPERTY(BlueprintReadWrite,Category="Stash | Component")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadWrite,Category="Stash | Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadWrite,Category="Stash | Default")
	float StashResetTime = 30.0f;

	UPROPERTY(BlueprintReadWrite,Category="Stash | Default")
	TArray<TSubclassOf<ABase_Pickup>> PossibleItemsToSpawn;
};



