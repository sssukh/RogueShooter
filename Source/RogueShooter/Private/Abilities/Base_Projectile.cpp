// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Base_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "Particles/ParticleSystem.h"
#include "Utility/RSLog.h"


// Sets default values
ABase_Projectile::ABase_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");

	SphereComponent->SetSphereRadius(32.0f);

	SphereComponent->SetLineThickness(0.0f);

	SetRootComponent(SphereComponent);

	ProjectileStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	
	ProjectileStaticMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	ProjectileMovement->InitialSpeed = 2000.0f;

	ProjectileMovement->MaxSpeed = 2000.0f;

	ProjectileMovement->bInitialVelocityInLocalSpace = true;

	ProjectileMovement->ProjectileGravityScale = 0.0f;

	ProjectileMovement->Bounciness = 0.6f;

	ProjectileMovement->Friction = 0.2;

	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 5.0f;
	
	ProjectileMovement->bRotationFollowsVelocity =  true;

}

// Called when the game starts or when spawned
void ABase_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ABase_Projectile::OnSphereOverlap);
}

// Called every frame
void ABase_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABase_Projectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner()) return; // 나 자신은 무시

	// 1. 적의 ASC 찾기
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	// 2. 명세서(Spec)가 유효하고 적에게 ASC가 있다면 적용
	if (DamageEffectSpecHandle.IsValid() && TargetASC)
	{
		// 내(Instigator)가 만든 명세서를 적(Target)에게 적용한다.
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}

	// 3. 투사체 파괴 (관통형이면 여기서 파괴 안 하고 횟수 차감 등 로직 추가)
	Destroy();
}

void ABase_Projectile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
                                 bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// UGameplayStatics::ApplyDamage(Hit.GetActor(),Damage,GetInstigator()->GetInstigatorController(),this,nullptr);

	RS_LOG_SCREEN(TEXT("Projectile Hit"))
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Hit_VFX,HitLocation);

	Destroy();
}

