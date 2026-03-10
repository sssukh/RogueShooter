// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RsPiercingProjectile.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ARsPiercingProjectile::ARsPiercingProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARsPiercingProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner()) return; // 나 자신은 무시
	
	ApplyDamage(OtherActor);
}

void ARsPiercingProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Hit_VFX,GetActorLocation());
	Destroy();
}

// Called when the game starts or when spawned
void ARsPiercingProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARsPiercingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

