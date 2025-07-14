// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Base_Chest.generated.h"

UCLASS()
class ROGUESHOOTER_API ABase_Chest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABase_Chest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// when pawn overlaps, do chest action and destroy chest
	// 폰이 sphere에 겹쳐지면 chest action을 호출하고 chest를 파괴한다.
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// When chest is overlapped - show chest UI for all players that are alive
	// chest에 겹쳐지면 - chest UI를 모든 살아있는 플레이어들에게 보인다.
	void ChestAction();
	
private:
	UPROPERTY()
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	
};
