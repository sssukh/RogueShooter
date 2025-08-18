// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActors/Gold_Pickup.h"

#include "Components/SphereComponent.h"
#include "Interface/Interface_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"


// Sets default values
AGold_Pickup::AGold_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	P_DeadMans_Loot = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("P_DeadMans_Loot"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(*AssetPath::EffectTemplate::DeadMansLoot);

	if(ParticleFinder.Succeeded())
		P_DeadMans_Loot->SetTemplate( ParticleFinder.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(*AssetPath::Mesh::SM_TD_Gold_Pile);

	if(StaticMeshFinder.Succeeded())
		StaticMesh->SetStaticMesh(StaticMeshFinder.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder(*AssetPath::Material::GoldMaterial);

	if(MatFinder.Succeeded())
		StaticMesh->SetMaterial(0,MatFinder.Object);
	
	StaticMesh->SetRelativeScale3D(FVector(0.5f));
	
	Sphere->SetRelativeScale3D(FVector(2.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectParticleFinder(*AssetPath::EffectTemplate::GoldSparkle);

	if(EffectParticleFinder.Succeeded())
		Particle = EffectParticleFinder.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase> EffectSoundFinder(*AssetPath::Sound::GoldPickup);

	if(EffectSoundFinder.Succeeded())
		PickupSound = EffectSoundFinder.Object;
}

// Called when the game starts or when spawned
void AGold_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGold_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGold_Pickup::DoPickupAction(AActor* Character)
{
	Super::DoPickupAction(Character);

	if(Character->GetInstigatorController()->GetClass()->ImplementsInterface(UInterface_PlayerState::StaticClass()))
	{
		IInterface_PlayerState::Execute_OnGoldPickUp(Character->GetInstigatorController(),GoldAmount);
	}
	else
	{
		RS_LOG_ERROR(TEXT("PlayerState가 PlayerState 인터페이스를 상속받지 않았습니다."))
	}
}

void AGold_Pickup::MC_PickupEffects()
{
	Super::MC_PickupEffects();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Particle,GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),PickupSound,GetActorLocation());
}

