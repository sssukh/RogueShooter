// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Base_Enemy.h"

#include "AudioDevice.h"
#include "AI/Base_AIController.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayActors/Base_Chest.h"
#include "GameplayActors/FloatingTextActor.h"
#include "GameplayActors/Soul.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RogueShooter/AssetPath.h"



// Sets default values
ABase_Enemy::ABase_Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);

	GetCapsuleComponent()->SetLineThickness(0.0f);

	SetRootComponent(GetCapsuleComponent());

	// Collision Sphere
	AttackCollisionSphere = CreateDefaultSubobject<USphereComponent>("AttackCollisionSphere");

	AttackCollisionSphere->SetSphereRadius(32.0f);

	AttackCollisionSphere->SetLineThickness(0.0f);

	AttackCollisionSphere->SetRelativeLocation(FVector(50.0f,0.0f,0.0f));
	
	AttackCollisionSphere->OnComponentBeginOverlap.AddDynamic(this,&ABase_Enemy::AttackSphereBeginOverlap);

	AttackCollisionSphere->OnComponentEndOverlap.AddDynamic(this,&ABase_Enemy::AttackSphereEndOverlap);

	AttackCollisionSphere->SetupAttachment(GetRootComponent());

	// Skeletal Mesh setting
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMeshFinder(*AssetPath::Mesh::BaseEnemyMesh);

	if(EnemyMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EnemyMeshFinder.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-90.0f));

	GetMesh()->SetRelativeRotation(FRotator(0.0f,270.0f,0.0f));
	
	// AnimInstance Class setting
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimFinder(*AssetPath::Animation::BaseEnemyAnim);

	if(AnimFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimFinder.Class);
	}
	
	// Particle System
	EliteAura = CreateDefaultSubobject<UParticleSystemComponent>("EliteAura");

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleFinder(*AssetPath::EffectTemplate::BaseEnemy);

	if(ParticleFinder.Succeeded())
	{
		EliteAura->SetTemplate( ParticleFinder.Object);
	}

	EliteAura->SetHiddenInGame(true);

	EliteAura->SetRelativeLocation(FVector(0.0f,0.0f,-90.0f));
	
	EliteAura->SetupAttachment(GetCapsuleComponent());


	// Variables Asset Initialization
	static ConstructorHelpers::FObjectFinder<USoundBase> AttackSoundFinder(*AssetPath::Sound::CueEnemyAttack);
	if(AttackSoundFinder.Succeeded())
	{
		EnemySound = AttackSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ImpactSoundFinder(*AssetPath::Sound::CueImpact);
	if(ImpactSoundFinder.Succeeded())
	{
		EnemySound = ImpactSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageFinder(*AssetPath::Montage::EnemyMelee);
	if(AttackMontageFinder.Succeeded())
	{
		AttackAnimation = AttackMontageFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageFinder(*AssetPath::Montage::GruntlingDeath);
	if(DeathMontageFinder.Succeeded())
	{
		DeathAnimation = DeathMontageFinder.Object;
	}

	DoOnce.Reset();

	TakeDamageDoOnce.Reset();

	ConstructorHelpers::FClassFinder<ASoul> SoulClassFinder(*AssetPath::Blueprint::BP_BaseSoul_C);
	if(SoulClassFinder.Succeeded())
	{
		SoulClass = SoulClassFinder.Class;
	}

	ConstructorHelpers::FClassFinder<AFloatingTextActor> FTActorClassFinder(*AssetPath::Blueprint::BP_FloatingTextActor_C);
	if(FTActorClassFinder.Succeeded())
	{
		FTActorClass = FTActorClassFinder.Class;
	}
}

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();

	DamageSphereOverlapDelegate.BindUFunction(this,FName("DamagePlayer"));
	RetriggerDelegate.BindUFunction(this,FName("ResetDoOnce"));
}

// Called every frame
void ABase_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABase_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABase_Enemy::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
		return;
	
	PlayerToDamage = OtherActor;

	// 1.0초 뒤에 DamageDelegate에 bind된 DamagePlayer() 호출한다.
	GetWorld()->GetTimerManager().SetTimer(
		DamagePlayerTimerReference,
		DamageSphereOverlapDelegate,
		1.0f,
		false);
	
}

void ABase_Enemy::AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> OverlappingActors;
	AttackCollisionSphere->GetOverlappingActors(OverlappingActors);

	if(OverlappingActors.IsEmpty())
		GetWorldTimerManager().ClearTimer(DamagePlayerTimerReference);
		
}

void ABase_Enemy::DamagePlayer()
{
	// 일정 시간동안 한번만 실행 
	if(!DoOnce.Execute())
		return;

	// 죽은 상태가 아닐 때 
	if(bIsDead)
		return;

	// Player에게 Damage만큼 데미지를 입힌다.
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	PlayerToDamage->TakeDamage(Damage, DamageEvent, nullptr, this);

	MC_EnemyAttack();

	SetTimerWithDelay(0.9f,false);
}

void ABase_Enemy::MC_EnemyAttack()
{
	PlayAnimMontage(AttackAnimation);

	if(GetWorld())
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),EnemySound,GetActorLocation());
	//EnemySoundHandle->PlaySoundAtLocation(EnemySound,GetWorld(),1.0f,1.0f,0.0f,GetActorLocation(),FRotator());
}

void ABase_Enemy::MC_OnHit()
{
	if(GetWorld())
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactSound,GetActorLocation());
}

void ABase_Enemy::MC_ShowAura()
{
	if(bIsElite)
	{
		EliteAura->SetHiddenInGame(false);
	}
}

float ABase_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	SpawnFloatingText(DamageAmount);

	MC_OnHit();

	// 피격당한 상황을 보이게한다.
	GetCharacterMovement()->StopMovementKeepPathing();

	Health = Health - DamageAmount;

	if(Health<=0)
	{
		if(TakeDamageDoOnce.Execute())
		{
			bIsDead =  true;

			// On Death 호출

			SpawnSoul();

			GetCharacterMovement()->StopMovementImmediately();

			BaseControllerReference->StopMovement();

			BaseControllerReference->EndAI();

			MC_Enemy_Death();

			DetachFromControllerPendingDestroy();
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABase_Enemy::MC_Enemy_Death()
{
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);

	// 새로 추가한 Enemy, projectile 채널 설정 
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_ENEMY,ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PROJECTILE,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(COLLISION_ENEMY,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PROJECTILE,ECR_Ignore);

	AttackCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayAnimMontage(DeathAnimation);

	FTimerHandle DelayHandle;

	// 1.5초 뒤에 authority있으면 destroy
	GetWorldTimerManager().SetTimer(
		DelayHandle,
		FTimerDelegate::CreateLambda([this]()
		{
			// TODO : 임시로 authority 빼둠 
			// if(HasAuthority())
				Destroy();
		}),
		1.5f,
		false);

	
}

void ABase_Enemy::SetTimerWithDelay(float Time, bool bLoop)
{
	GetWorldTimerManager().ClearTimer(RetriggerHandle);
	GetWorldTimerManager().SetTimer(RetriggerHandle,RetriggerDelegate,Time,bLoop);
}

void ABase_Enemy::ResetDoOnce()
{
	DoOnce.Reset();
}

void ABase_Enemy::SpawnFloatingText(float InDamage)
{
	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.X+=FMath::RandRange(-10.0f,10.0f);
	SpawnLocation.Y+=FMath::RandRange(-10.0f,10.0f);
	SpawnLocation.Z+=FMath::RandRange(-10.0f,10.0f);

	if(AFloatingTextActor* FloatingTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FTActorClass,FTransform(SpawnLocation)))
	{
		FloatingTextActor->Damage = InDamage;
		FloatingTextActor->FinishSpawning(FTransform(SpawnLocation));
	}
}

void ABase_Enemy::SpawnSoul()
{
	if(ASoul* SoulSpawn = GetWorld()->SpawnActorDeferred<ASoul>(SoulClass,FTransform(GetActorLocation())))
	{
		SoulSpawn->GM_Interface = this->GM_Interface;
		SoulSpawn->FinishSpawning(FTransform(GetActorLocation()));
	}

	if(bIsElite)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z -=100.0f;
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator Rotator = FRotator();
		GetWorld()->SpawnActor(ABase_Chest::StaticClass(),&SpawnLocation,&Rotator,ActorSpawnParameters);
	}
}





