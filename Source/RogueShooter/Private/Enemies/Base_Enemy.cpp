// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Base_Enemy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AI/Base_AIController.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Data/Attribute/CombatSet.h"
#include "Data/Attribute/HealthSet.h"
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
#include "BrainComponent.h"
#include "Components/MonsterPoolComponent.h"
#include "Components/WaveManagerComponent.h"
#include "Data/Attribute/ExpSet.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerState.h"
#include "System/RsHUD.h"
#include "UObject/FastReferenceCollector.h"


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

	// static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageFinder(*AssetPath::Montage::GruntlingDeath);
	// if(DeathMontageFinder.Succeeded())
	// {
	// 	DeathAnimation = DeathMontageFinder.Object;
	// }

	DoOnce.Reset();

	TakeDamageDoOnce.Reset();

	static ConstructorHelpers::FClassFinder<ASoul> SoulClassFinder(*AssetPath::Blueprint::BP_BaseSoul_C);
	if(SoulClassFinder.Succeeded())
	{
		SoulClass = SoulClassFinder.Class;
	}

	

	static ConstructorHelpers::FClassFinder<ABase_AIController> AIControllerFinder(*AssetPath::Blueprint::BP_Base_AIController_C);
	if(AIControllerFinder.Succeeded())
	{
		AIControllerClass = AIControllerFinder.Class;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthSet = CreateDefaultSubobject<UHealthSet>(TEXT("HeathAttributes"));
	CombatSet = CreateDefaultSubobject<UCombatSet>(TEXT("CombatAttributes"));
	
	
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
		
		ApplyAttributeOnLevel(CharLevel);
		
		SetupAbilitiesAndAttributes();
		
			
		
		// RS_LOG_SCREEN(TEXT("Health : %f"),AbilitySystemComponent->GetNumericAttribute(HealthSet->GetHealthAttribute()))
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

	MC_OnHit();

	// 피격당한 상황을 보이게한다.
	GetCharacterMovement()->StopMovementKeepPathing();


	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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

	// PlayAnimMontage(DeathAnimation);

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




void ABase_Enemy::Die(AActor* DamageCauser)
{
	if (!DamageCauser)
		return;
	
	bIsDead = true;
	
	ApplyXpToTargetPlayer(DamageCauser);
	
	DeactivateToPool();
	
	
	
	// 몬스터 체력 바 풀링으로 되돌리기 
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// 확실한 내 화면일 때만 HUD에 갱신 요청!
		if (ARsHUD* MyHUD = Cast<ARsHUD>(PC->GetHUD()))
		{
			MyHUD->HideMonsterHealthBar(this);
		}
	}
}

void ABase_Enemy::ApplyXpToTargetPlayer(AActor* TargetPlayer)
{
	
	// 죽인 유저의 ASC
	UAbilitySystemComponent* TargetASC  = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetPlayer);
	
	if (!TargetASC)
		return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DropExpClass,CharLevel,ContextHandle);
	
	
	if (SpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data,TargetASC);
	}
	
	// 전송 주기 무시하고 전송 
	if (APlayerState* PS = Cast<APlayerState>(TargetASC->GetOwner()))
	{
		PS->ForceNetUpdate();
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

void ABase_Enemy::ShowEliteAura()
{
	if(bIsElite)
	{
		EliteAura->SetHiddenInGame(false,false);
	}
}



bool ABase_Enemy::IsAlive_Implementation()
{
	// return IInterface_CharacterManager::IsAlive_Implementation();
	return !bIsDead;
}

void ABase_Enemy::SetupAbilitiesAndAttributes()
{
	AddCharacterAbilities();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddUObject(this,&ABase_Enemy::OnHealthChanged);
}

void ABase_Enemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	// 1. 방어: 데디케이티드 서버라면 UI 작업은 쳐다보지도 말고 즉시 종료!
	if (GetNetMode() == NM_DedicatedServer) return;

	// 2. 방어: 이 몬스터가 죽었거나 숨겨진 상태(풀링 대기열)라면 무시!
	if (IsHidden() || bIsDead) return;

	
	// 3. 로컬 플레이어 찾기 (클라이언트 환경이므로 GetFirstPlayerController가 내 컨트롤러임)
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// 확실한 내 화면일 때만 HUD에 갱신 요청!
		if (ARsHUD* MyHUD = Cast<ARsHUD>(PC->GetHUD()))
		{
			if (Data.NewValue <=0.0f)
			{
				MyHUD->HideMonsterHealthBar(this);
			}
			else
			{
				// 중앙 집중형 HUD에게 "나 체력 변했으니 포스트잇(체력바) 좀 붙여줘!" 라고 요청
				MyHUD->UpdateMonsterHealthBar(this, Data.NewValue); 
			}
			
		}
	}
	
}

void ABase_Enemy::ActivateFromPool()
{
	
	// 1. 렌더링 및 충돌 복구
	SetActorHiddenInGame(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    
	// 2. 틱 및 이동 연산 복구
	SetActorTickEnabled(true);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// 3. AI 비헤비어 트리 재시작
	
	if (ABase_AIController* MyAICon = Cast<ABase_AIController>(GetController()))
	{
		MyAICon->ActivateAI(); // 예: RunBehaviorTree(MyTree) 를 호출하는 함수
	}
	
	// if (AAIController* AICon = Cast<AAIController>(GetController()))
	// {
	// 	if (AICon->GetBrainComponent())
	// 	{
	// 		AICon->GetBrainComponent()->RestartLogic();
	// 	}
	// 	else
	// 	{
	// 		// 🌟 핵심: 뇌가 아예 없다면(풀에 들어가느라 초기화가 안 됐다면), 
	// 		// AI 컨트롤러에 직접 접근해서 비헤비어 트리를 처음부터 강제로 켜주어야 합니다!
	// 		// (AMyAIController는 실제 사용 중인 AI컨트롤러 클래스명으로 변경, RunAI는 BT를 실행하는 사용자 정의 함수)
	// 		if (ABase_AIController* MyAICon = Cast<ABase_AIController>(AICon))
	// 		{
	// 			MyAICon->ActivateAI(); // 예: RunBehaviorTree(MyTree) 를 호출하는 함수
	// 		}
	// 	}
	// }

	// TODO: GAS 초기화 (체력 100% 복구 GE 재적용 등)
	// TODO : 웨이브정보에 몬스터 레벨을 추가
	ApplyAttributeOnLevel(CharLevel);
}

void ABase_Enemy::DeactivateToPool()
{
	// 1. 렌더링 및 충돌 해제
	// SetActorHiddenInGame(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	
	// 2. 틱 및 이동 연산 중지
	SetActorTickEnabled(false);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// 3. AI 비헤비어 트리 중지
	if (ABase_AIController* AICon = Cast<ABase_AIController>(GetController()))
	{
		AICon->DeactivateAI(); // 🌟 여기서 타이머와 BT를 확실하게 중지
	}
	
	

	// TODO: GAS가 적용되어 있다면 진행 중인 Ability와 Gameplay Effect(DoT 데미지 등)를 모두 취소하는 로직 추가
	// AbilitySystemComponent->CancelAllAbilities();
	// AbilitySystemComponent->RemoveAllGameplayEffects();
}

void ABase_Enemy::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ABase_Enemy::OnDeathMontageEnded);
	}
	
	// 몬스터 객체 풀로 반환 
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (UWaveManagerComponent* WaveManagerComponent = GameMode->FindComponentByClass<UWaveManagerComponent>())
		{
			WaveManagerComponent->OnMonsterDied(this);
		}
		
		if (UMonsterPoolComponent* MonsterPoolComponent = GameMode->FindComponentByClass<UMonsterPoolComponent>())
		{
			MonsterPoolComponent->ReturnMonster(this);
		}
	}
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







