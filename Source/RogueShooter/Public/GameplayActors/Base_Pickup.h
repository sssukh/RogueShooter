// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Pickup.generated.h"

class USphereComponent;

UCLASS()
class ROGUESHOOTER_API ABase_Pickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Pickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DoPickupAction(AActor* Character);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Base Pickup | Component")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Base Pickup | Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

};
