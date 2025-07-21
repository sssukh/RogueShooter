// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LightningExplosion.h"
#include "Particles/ParticleSystem.h"
#include "RogueShooter/AssetPath.h"


// Sets default values
ALightningExplosion::ALightningExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LightningEffectFinder(*AssetPath::EffectTemplate::LightningEffect);

	if(LightningEffectFinder.Succeeded())
	{
		DamageVFX = LightningEffectFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> LightningSoundFinder(*AssetPath::Sound::LightningSound);

	if(LightningSoundFinder.Succeeded())
	{
		SoundVFX = LightningSoundFinder.Object;
	}

	Damage = 1.0f;

	Radius = 100.0f;
}

// Called when the game starts or when spawned
void ALightningExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightningExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

