// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Base_Enemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AudioDevice.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AI/Base_AIController.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Data/CombatSet.h"
#include "Data/HealthSet.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayActors/Base_Chest.h"
#include "GameplayActors/FloatingTextActor.h"
#include "GameplayActors/Soul.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/FRsGameplayTags.h"
#include "Utility/RSCollisionChannel.h"
#include "Utility/RSLog.h"
#include "GameplayAbilitiesModule.h"
#include "AbilitySystemGlobals.h"
#include "Data/ExpSet.h"


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

	static ConstructorHelpers::FClassFinder<ASoul> SoulClassFinder(*AssetPath::Blueprint::BP_BaseSoul_C);
	if(SoulClassFinder.Succeeded())
	{
		SoulClass = SoulClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AFloatingTextActor> FTActorClassFinder(*AssetPath::Blueprint::BP_FloatingTextActor_C);
	if(FTActorClassFinder.Succeeded())
	{
		FTActorClass = FTActorClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<ABase_AIController> AIControllerFinder(*AssetPath::Blueprint::BP_Base_AIController_C);
	if(AIControllerFinder.Succeeded())
	{
		AIControllerClass = AIControllerFinder.Class;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	ScaleHP();
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthAttributes = CreateDefaultSubobject<UHealthSet>(TEXT("HeathAttributes"));
	CombatAttributes = CreateDefaultSubobject<UCombatSet>(TEXT("CombatAttributes"));
	
	AbilitySystemComponent->AddAttributeSetSubobject<UHealthSet>(HealthAttributes);
	AbilitySystemComponent->AddAttributeSetSubobject<UCombatSet>(CombatAttributes);
	
	AbilitySystemComponent->SetIsReplicated(true);
	// 예측없이 서버가 시키는 대로
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

// Called when the game starts or when spawned
void ABase_Enemy::BeginPlay()
{
	Super::BeginPlay();

	
	DamageSphereOverlapDelegate.BindUObject(this,&ABase_Enemy::DamagePlayer);
	RetriggerDelegate.BindUObject(this,&ABase_Enemy::ResetDoOnce);

	// UE_LOG(LogTemp, Warning, TEXT("Enemy %s BeginPlay, Controller: %s"),
	// 	*GetName(),
	// 	GetController() ? *GetController()->GetName() : TEXT("NULL"));
	
	// AbilitySystem 초기화 
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		
		AddCharacterAbilities();
		
		
		IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(AbilitySystemComponent,TEXT("Enemy"),CharLevel,true);
	}
}

// Called every frame
void ABase_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float LogInterval = 1.0f;
	static float TimeAccum = 0.0f;
	TimeAccum += DeltaTime;

	if (TimeAccum > LogInterval)
	{
		TimeAccum = 0.0f;
		// UE_LOG(LogTemp, Warning, TEXT("Tick: Enemy %s Controller: %s"),
		// 	*GetName(),
		// 	GetController() ? *GetController()->GetName() : TEXT("NULL"));
	}
}

// Called to bind functionality to input
void ABase_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABase_Enemy::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO : 적의 공격 우선순위를 정하는 알고리즘이 수정 필요 
	if(!OtherActor)
		return;
	
	PlayerToDamage = OtherActor;

	// 1.0초 뒤에 DamageDelegate에 bind된 DamagePlayer() 호출한다.
	GetWorld()->GetTimerManager().SetTimer(
		DamagePlayerTimerReference,
		DamageSphereOverlapDelegate,
		1.0f,
		true);

	DamagePlayer();
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

	// TODO : GA를 이용한 attack
	DamageWithGameplayTag();
	
	MC_EnemyAttack();

	SetTimerWithDelay(0.9f,false);
}

void ABase_Enemy::DamageWithGameplayTag()
{
	// 대상 유효성 체크
	if (!PlayerToDamage)
	{
		RS_LOG_ERROR(TEXT("Target Player is not set"))
		return;
	}
	
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(PlayerToDamage);
	if (ASCInterface)
	{
		UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();
		if (TargetASC)
		{
			RS_LOG_ERROR(TEXT("Event Hit Occur"))
			
			// 이벤트 데이터 포장
			FGameplayEventData Payload;
			Payload.EventTag = FRsGameplayTags::Get().Event_Hit;
			Payload.Instigator = this;
			Payload.Target = PlayerToDamage;

			// Enemy가 "Event.Hit"을 기다리는 GA(WaitGameplayEvent)를 켜놓고 있다면 반응함.
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Payload.EventTag, Payload);
		}
	}
}

void ABase_Enemy::MC_EnemyAttack_Implementation()
{
	PlayAnimMontage(AttackAnimation);

	if(GetWorld())
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),EnemySound,GetActorLocation());
	//EnemySoundHandle->PlaySoundAtLocation(EnemySound,GetWorld(),1.0f,1.0f,0.0f,GetActorLocation(),FRotator());
}




void ABase_Enemy::MC_OnHit_Implementation()
{
	if(GetWorld())
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(),ImpactSound,GetActorLocation());
}

void ABase_Enemy::MC_ShowAura_Implementation()
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

	// if(Health<=0)
	// {
	// 	// Event.Kill 전송
	// 	SendDeathEvent(DamageCauser);
	// 	// Interface TODO : 지금은 코드로 하지만 GAS로 하게되면 거기로 옮겨야됨 
	// 	CharDie();
	// }
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void ABase_Enemy::CharDie_Implementation(AActor* Causer)
{
	if(HasAuthority())
	{
		if (!Causer)
			return;
		if(TakeDamageDoOnce.Execute())
		{
			// Event.Combat.Kill 전송
			SendDeathEvent(Causer);
			
			bIsDead =  true;

			if(OnDeath.IsBound())
				OnDeath.Broadcast();
			
			
			SpawnSoul();

			GetCharacterMovement()->StopMovementImmediately();

			ABase_AIController* AIController = Cast<ABase_AIController>(GetController());

			if(AIController)
			{
				// RS_LOG_SCREEN(TEXT("%s is Dead on Server"),*GetName())
				
				AIController->StopMovement();

				AIController->EndAI();
			}

			MC_Enemy_Death();

			DetachFromControllerPendingDestroy();
		}
	}
}

void ABase_Enemy::MC_Enemy_Death_Implementation()
{
	GetCapsuleComponent()->SetCollisionObjectType(ECC_WorldDynamic);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECR_Ignore);

	// 새로 추가한 Enemy, projectile 채널 설정 
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);

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


void ABase_Enemy::SendDeathEvent(AActor* Killer)
{
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Killer);
	if (ASCInterface)
	{
		UAbilitySystemComponent* KillerASC = ASCInterface->GetAbilitySystemComponent();
		if (KillerASC)
		{
			RS_LOG_ERROR(TEXT("Event Death Occur"))
			
			// 이벤트 데이터 포장
			FGameplayEventData Payload;
			Payload.EventTag = FRsGameplayTags::Get().Event_Death;
			Payload.Instigator = this;
			Payload.Target = this;

			// [핵심] 킬러에게 이벤트를 쏘다!
			// Killer가 "Event.Kill"을 기다리는 GA(WaitGameplayEvent)를 켜놓고 있다면 반응함.
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Killer, Payload.EventTag, Payload);
		}
	}
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
	// FVector SpawnLocation = GetActorLocation();
	// SpawnLocation.X+=FMath::RandRange(-10.0f,10.0f);
	// SpawnLocation.Y+=FMath::RandRange(-10.0f,10.0f);
	// SpawnLocation.Z+=FMath::RandRange(-10.0f,10.0f);
	//
	// if(AFloatingTextActor* FloatingTextActor = GetWorld()->SpawnActorDeferred<AFloatingTextActor>(FTActorClass,FTransform(SpawnLocation)))
	// {
	// 	FloatingTextActor->Damage = InDamage;
	// 	FloatingTextActor->FinishSpawning(FTransform(SpawnLocation));
	// }
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

void ABase_Enemy::ShowEliteAura()
{
	if(bIsElite)
	{
		EliteAura->SetHiddenInGame(false,false);
	}
}

void ABase_Enemy::ScaleHP()
{
	if(ScaleHPToLevel)
	{
		Health = Health * CharLevel;
	}
}

bool ABase_Enemy::IsAlive_Implementation()
{
	// return IInterface_CharacterManager::IsAlive_Implementation();
	return !bIsDead;
}



void ABase_Enemy::AddCharacterAbilities()
{
	// 1. 서버 권한 확인 (중요: 클라이언트에서 실행하면 안 됨)
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
	{
		return;
	}

	// 2. 어빌리티 순회하며 부여
	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			// 3. Spec 생성 (클래스, 레벨, 입력ID, 소스)
			// 예시: 레벨 1, 입력 ID는 -1 (없음) 또는 Enum 값
			FGameplayAbilitySpec Spec(AbilityClass, 1, -1, this);

			// 4. 어빌리티 부여 (GiveAbility)
			// 리턴받은 Handle은 나중에 필요하면 저장해둡니다.
			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
			
			// AbilitySystemComponent->TryActivateAbility(Handle);
		}
	}
}

UAbilitySystemComponent* ABase_Enemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}







