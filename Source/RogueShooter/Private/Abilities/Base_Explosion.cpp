// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Base_Explosion.h"

#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSCollisionChannel.h"
#include "Particles/ParticleSystem.h"


// Sets default values
ABase_Explosion::ABase_Explosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UParticleSystem> DamageEffectFinder(*AssetPath::EffectTemplate::Explosion);

	if(DamageEffectFinder.Succeeded())
	{
		DamageVFX = DamageEffectFinder.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> DamageSoundfinder(*AssetPath::Sound::ExplosionSound);

	if(DamageSoundfinder.Succeeded())
	{
		SoundVFX = DamageSoundfinder.Object;
	}
}

// Called when the game starts or when spawned
void ABase_Explosion::BeginPlay()
{
	Super::BeginPlay();

	MC_VFX_Implementation();

	if(!HasAuthority())
	{
		return;
	}

	TArray<FHitResult> HitResults;

	FVector ActorLocation = GetActorLocation();	

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_ENEMY);
	
	GetWorld()->SweepMultiByObjectType(HitResults,ActorLocation,ActorLocation,FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(Radius));

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(),ActorLocation,Radius,32,FColor::Blue,false,2.0f);
#endif

	UFunctionLibrary_Helper::DamageEnemiesOnce(GetWorld(),HitResults,Damage,GetInstigator()->GetInstigatorController(),this);

	SetLifeSpan(0.5f);
}

// Called every frame
void ABase_Explosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Explosion::MC_VFX_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),DamageVFX,GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),SoundVFX,GetActorLocation());
}

