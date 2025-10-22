// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "Soul.generated.h"

class UProjectileMovementComponent;
class FOnTimelineEvent;
class FOnTimelineFloat;
struct FDoOnce;
class UInterface_GameManager;
class USphereComponent;

UCLASS()
class ROGUESHOOTER_API ASoul : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoul();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION(BlueprintCallable,Category= "Soul")
	void AddXP();

	UFUNCTION(Category = "On Overlap Event")
	void InnerSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "On Overlap Event")
	void OuterSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	TObjectPtr<USphereComponent> Inner;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	TObjectPtr<AActor> OtherActor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	FVector StartingLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Soul | Component")
	int32 XPAmount = 25;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Soul | Component", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UObject> GM_Interface;

	UPROPERTY(EditAnywhere)
	FDoOnce DoOnce;
	
	UPROPERTY(EditAnywhere)
	FDoOnce SphereDoOnce;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UParticleSystem> EmitterTemplate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Soul | Setting")
	float FollowingSpeed = 6.0f;
	
	FVector Destination;

	bool bIsFollowing = false;
};
