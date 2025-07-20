// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Base_Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"


// Sets default values
ABase_Projectile::ABase_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");

	Sphere->SetSphereRadius(32.0f);

	Sphere->SetLineThickness(0.0f);

	SetRootComponent(Sphere);

	Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(*AssetPath::Mesh::Cube);
	if(CubeFinder.Succeeded())
	{
		Cube->SetStaticMesh(CubeFinder.Object);
		Cube->SetRelativeScale3D(FVector(0.5f,0.05f,0.05f));
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> MatFinder(*AssetPath::Material::Blue);
	if(MatFinder.Succeeded())
	{
		Cube->SetMaterial(0,MatFinder.Object);
	}

	Cube->SetupAttachment(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	ProjectileMovement->InitialSpeed = 2000.0f;

	ProjectileMovement->MaxSpeed = 2000.0f;

	ProjectileMovement->bInitialVelocityInLocalSpace = true;

	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ProjectileMovement->Bounciness = 0.6f;

	ProjectileMovement->Friction = 0.2;

	ProjectileMovement->BounceVelocityStopSimulatingThreshold - 5.0f;

	ConstructorHelpers::FObjectFinder<UParticleSystem> EffectFinder(*AssetPath::EffectTemplate::ImpactImce);

	if(EffectFinder.Succeeded())
	{
		Hit_VFX = EffectFinder.Object;
	}
}

// Called when the game starts or when spawned
void ABase_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Projectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UGameplayStatics::ApplyDamage(Hit.GetActor(),Damage,GetInstigator()->GetInstigatorController(),this,nullptr);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Hit_VFX,HitLocation);

	Destroy();
}

