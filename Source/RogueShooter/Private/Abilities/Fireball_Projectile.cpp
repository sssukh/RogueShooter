// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Fireball_Projectile.h"

#include "Abilities/Base_Explosion.h"
#include "RogueShooter/AssetPath.h"


// Sets default values
AFireball_Projectile::AFireball_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	static ConstructorHelpers::FClassFinder<ABase_Explosion> ExplosionClassFinder(*AssetPath::Blueprint::BP_BaseExplosion_C);
	if(ExplosionClassFinder.Succeeded())
	{
		ExplosionClass = ExplosionClassFinder.Class;
	}

	
}

// Called when the game starts or when spawned
void AFireball_Projectile::BeginPlay()
{
	Super::BeginPlay();
	Cube->SetMaterial(0,FireballMaterial);
}

// Called every frame
void AFireball_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireball_Projectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
	class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	FTransform explosionTransform;
	explosionTransform.SetIdentity();
	explosionTransform.SetLocation(GetActorLocation());
	
	if(ABase_Explosion* explosion = GetWorld()->SpawnActorDeferred<ABase_Explosion>(ExplosionClass,explosionTransform,nullptr,GetInstigator(),ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		explosion->Radius = Radius;
		explosion->Damage = Damage;
		explosion->DamageVFX = Hit_VFX;
		explosion->FinishSpawning(explosionTransform);
	}
}

