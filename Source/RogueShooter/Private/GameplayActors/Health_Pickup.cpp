// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Health_Pickup.h"

#include "Interface/Interface_CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"


// Sets default values
AHealth_Pickup::AHealth_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(*AssetPath::EffectTemplate::HealthPickup);

	if(ParticleFinder.Succeeded())
		Particle = ParticleFinder.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(*AssetPath::Sound::HealthPickup);

	if(SoundFinder.Succeeded())
		PickupSound = SoundFinder.Object;

	
}

// Called when the game starts or when spawned
void AHealth_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealth_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealth_Pickup::DoPickupAction(AActor* Character)
{
	Super::DoPickupAction(Character);

	if(Character->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		IInterface_CharacterManager::Execute_RestoreHealth(Character,HealthAmount);
	}
	else
	{
		RS_LOG_ERROR(TEXT("Character가 CharacterManager 인터페이스를 상속받지 않았습니다."))
	}
}

void AHealth_Pickup::MC_PickupEffects()
{
	Super::MC_PickupEffects();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Particle,GetActorLocation(),FRotator::ZeroRotator,FVector(2.0f));

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation());
}

