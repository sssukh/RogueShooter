// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilitiesComponent.h"

#include "Net/UnrealNetwork.h"
#include "RogueShooter/RSEnumStruct.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Base_Projectile.h"
#include "Abilities/Fireball_Projectile.h"
#include "Abilities/LightningExplosion.h"
#include "Character/Base_Character.h"
#include "Engine/OverlapResult.h"
#include "Interface/Interface_CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/RSCollisionChannel.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"
#include "Particles/ParticleSystem.h"
#include "Saves/SG_Player.h"


// Sets default values for this component's properties
UAbilitiesComponent::UAbilitiesComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	
	// ...

	static ConstructorHelpers::FObjectFinder<UParticleSystem> HammerEmitterFinder(*AssetPath::EffectTemplate::HammerEffect);
	if(HammerEmitterFinder.Succeeded())
	{
		HammerEffect = HammerEmitterFinder.Object;	
	}

	ConstructorHelpers::FObjectFinder<USoundBase> HammerSoundFinder(*AssetPath::Sound::HammerSound);
	if(HammerSoundFinder.Succeeded())
	{
		HammerSound = HammerSoundFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> FrostBoltEmitterFinder(*AssetPath::EffectTemplate::FrostBoltEffect);
	if(FrostBoltEmitterFinder.Succeeded())
	{
		 FrostBoltEffect = FrostBoltEmitterFinder.Object;	
	}

	ConstructorHelpers::FObjectFinder<USoundBase>FrostBoltSoundFinder(*AssetPath::Sound::FrostBoltSound);
	if(FrostBoltSoundFinder.Succeeded())
	{
		 FrostBoltSound = FrostBoltSoundFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> FireballEmitterFinder(*AssetPath::EffectTemplate::FireballEffect);
	if(FireballEmitterFinder.Succeeded())
	{
		FireballEffect = FireballEmitterFinder.Object;	
	}

	ConstructorHelpers::FObjectFinder<USoundBase> FireballSoundFinder(*AssetPath::Sound::FireballSound);
	if(FireballSoundFinder.Succeeded())
	{
		FireballSound = FireballSoundFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> LightningEmitterFinder(*AssetPath::EffectTemplate::LightningEffect);
	if(LightningEmitterFinder.Succeeded())
	{
		LightningEffect = LightningEmitterFinder.Object;	
	}

	ConstructorHelpers::FObjectFinder<USoundBase> LightningSoundFinder(*AssetPath::Sound::LightningSound);
	if(LightningSoundFinder.Succeeded())
	{
		LightningSound = LightningSoundFinder.Object;
	}

	
	ConstructorHelpers::FClassFinder<ABase_Projectile> ProjectileClassFinder(*AssetPath::Blueprint::BP_BaseProjectile_C);
	if(ProjectileClassFinder.Succeeded())
	{
		BaseProjectileClass = ProjectileClassFinder.Class;
	}
	
	ConstructorHelpers::FClassFinder<ALightningExplosion> LightningClassFinder(*AssetPath::Blueprint::BP_LightningExplosion_C);
	if(LightningClassFinder.Succeeded())
	{
		LightningClass = LightningClassFinder.Class;
	}
	
	ConstructorHelpers::FClassFinder<AFireball_Projectile> FireballClassFinder(*AssetPath::Blueprint::BP_FireballProjectile_C);
	if(FireballClassFinder.Succeeded())
	{
		FireballClass = FireballClassFinder.Class;
	}
}


// Called when the game starts
void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO : component 생성할 때 set해야한다고 한다.
	SetIsReplicated(true);
	// ...

	if (GetClass()->FindFunctionByName(FName("PrepareHammer")) == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("리플렉션에서 PrepareHammer를 찾을 수 없습니다!"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("리플렉션에 PrepareHammer 정상 등록됨."));
	}
	
	
	
}


// Called every frame
void UAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilitiesComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilitiesComponent,AbilityTimerMultiplier)
	DOREPLIFETIME(UAbilitiesComponent,AbilityDamageMultiplier)
	DOREPLIFETIME(UAbilitiesComponent,HammerTimer)
}

float UAbilitiesComponent::CalculateTimerMode(float InMultiplyer)
{
	return InMultiplyer * AbilityTimerMultiplier;
}

float UAbilitiesComponent::CalculateBonusDamage(float InMultiplyer)
{
	return InMultiplyer * AbilityDamageMultiplier;
}

bool UAbilitiesComponent::CheckEvoActive(EActiveAbilities InEnumActive)
{
	if(EvolutionTracker.IsEmpty())
		return false;
	
	return EvolutionTracker[(int32)InEnumActive];
}

void UAbilitiesComponent::LevelUpHammer()
{
	int32 HammerLevel = LevelUpActive(EActiveAbilities::Hammer);

	switch(HammerLevel)
	{
	case 1:
		GrantHammer(true);
		break;
	case 2:
	case 4:
		HammerDamage = HammerDamage+5;
		break;
	case 3:
		HammerRadius *= 1.1f;
		break;
	case 5:
		HammerRadius *= 1.1f;
		EvolutionMap.Add(EActiveAbilities::Hammer,EPassiveAbilities::Ability_Bonus_Damage);
		break;
	default:
		break;
	}
}

void UAbilitiesComponent::GrantHammer(bool Cast)
{
	HammerDelegate.BindUFunction(this,FName("PrepareHammer"));
	if(!HammerDelegate.IsBound())
	{
		RS_LOG_ERROR(TEXT("바운드 실패"))
	}
	
	FTimerHandle HammerTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HammerTimerHandle, HammerDelegate,CalculateTimerMode(HammerTimer),true);

	
	
	ActiveTimers.AddUnique(HammerTimerHandle);

	if(Cast)
		PrepareHammer();
}

void UAbilitiesComponent::PrepareHammer()
{
	TArray<FHitResult> OutHits;
	
	FVector ActorLocation = GetOwner()->GetActorLocation();

	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_ENEMY);
	
	
	bool bHit = GetWorld()->SweepMultiByObjectType(OutHits,ActorLocation,ActorLocation,FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(HammerRadius),params);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(),ActorLocation,HammerRadius,32,FColor::Red,false,2.0f);
#endif

	float damage = CalcAbilityDamageWithCrit(0.15f,EActiveAbilities::Hammer,HammerDamage);
	S_ExecuteHammer_Implementation(OutHits,damage,HammerRadius,UGameplayStatics::GetPlayerController(GetWorld(),0));
}

void UAbilitiesComponent::S_ExecuteHammer_Implementation(const TArray<FHitResult>& Hits, float Damage, float Radius,
	APlayerController* Controller)
{
	MC_Hammer_Implementation(Radius);

	UFunctionLibrary_Helper::DamageEnemiesOnce(this,Hits,Damage,Controller,GetOwner());
}

void UAbilitiesComponent::LevelUpFrostBolt()
{
	switch(LevelUpActive(EActiveAbilities::FrostBolt))
	{
	case 1:
		GrantFrostBolt(true);
		break;
	case 2:
		++FBFireCount;
		break;
	case 3:
		FBTimer *=0.9f;
		break;
	case 4:
		FBDamage +=5.0f;
		break;
	case 5:
		++FBFireCount;
		EvolutionMap.Add(EActiveAbilities::FrostBolt,EPassiveAbilities::Health_Bonus);
		break;
	default:
		break;
	}

	
}

void UAbilitiesComponent::GrantFrostBolt(bool Cast)
{
	FrostBoltDelegate.BindUFunction(this,FName("PrepareFrostbolt"));
	if(!FrostBoltDelegate.IsBound())
	{
		RS_LOG_ERROR(TEXT("바운드 실패"))
	}
	
	FTimerHandle FrostTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FrostTimerHandle,FrostBoltDelegate,CalculateTimerMode(FBTimer),true);
	ActiveTimers.AddUnique(FrostTimerHandle);

	if(Cast)
	{
		PrepareFrostBolt();
	}
}

void UAbilitiesComponent::PrepareFrostBolt()
{
	FBFireIndex = 0;

	// IInterface 상속 검증 
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("User Character가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}

	USphereComponent* AbilitySphere = IInterface_CharacterManager::Execute_GetAbilitySphere(GetOwner());

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_ENEMY);
	
	bool bAnyHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,GetOwner()->GetActorLocation(),
		FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(AbilitySphere->GetScaledSphereRadius()));

	// 여기부터

	if(!bAnyHit)
	{
		// 없으면 함수 종료
		return;
	}

	TArray<AActor*> Actors;

	float distance;
	
	for(FOverlapResult overlap : OverlapResults)
	{
		Actors.AddUnique(overlap.GetActor());
	}
	
	ABase_Character* Char = IInterface_CharacterManager::Execute_GetCharacter(GetOwner());

	AActor* LocNearestActor = UGameplayStatics::FindNearestActor(GetOwner()->GetActorLocation(),Actors,distance);

	S_ExecuteFrostBolt_Implementation(LocNearestActor,Char,CalculateBonusDamage(FBDamage));

	// ExecuteFrostBolt 호출 후 0.05초 delay
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateLambda([&]()
	{
		if(FBFireIndex<=FBFireCount)
		{
			++FBFireIndex;
			S_ExecuteFrostBolt_Implementation(LocNearestActor,Char,CalculateBonusDamage(FBDamage));
		}
	}),
	0.05f,
	true
	);
}

void UAbilitiesComponent::LevelUpLightning()
{
	switch (LevelUpActive(EActiveAbilities::Lightning))
	{
	case 1 :
		GrantLightning(true);
		break;
	case 2:
	case 4:
		LightningDamage+=5.0f;
		break;
	case 3:
		LightningRadius*=1.1f;
		break;
	case 5:
		LightningRadius*=1.1f;
		EvolutionMap.Add(EActiveAbilities::Lightning,EPassiveAbilities::Ability_Cooldown_Reduction);
		break;
	default:
		break;
	}
}

void UAbilitiesComponent::GrantLightning(bool Cast)
{
	LightningDelegate.BindUFunction(this,FName("PrepareLightning"));
	if(!LightningDelegate.IsBound())
	{
		RS_LOG_ERROR(TEXT("바운드 실패"))
	}
	
	FTimerHandle LightningTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle,LightningDelegate,CalculateTimerMode(LightningTimer),true);
	ActiveTimers.AddUnique(LightningTimerHandle);

	if(Cast)
	{
		PrepareLightning();
	}
}

void UAbilitiesComponent::PrepareLightning()
{
	// IInterface 상속 검증 
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("User Character가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}

	USphereComponent* AbilitySphere = IInterface_CharacterManager::Execute_GetAbilitySphere(GetOwner());

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_ENEMY);
	
	bool bAnyHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,GetOwner()->GetActorLocation(),
		FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(AbilitySphere->GetScaledSphereRadius()));
	

	if(!bAnyHit)
	{
		// 없으면 함수 종료
		return;
	}

	TArray<AActor*> Actors;
	
	for(FOverlapResult overlap : OverlapResults)
	{
		Actors.AddUnique(overlap.GetActor());
	}
	
	ABase_Character* Char = IInterface_CharacterManager::Execute_GetCharacter(GetOwner());

	int32 RandIdx = FMath::RandRange(0,Actors.Num()-1);

	LastLightningLoc = Actors[RandIdx]->GetActorLocation();

	S_ExecuteLightning(LastLightningLoc,Char,CalculateBonusDamage(LightningDamage),LightningRadius);

	// Evolution

	if(!CheckEvoActive(EActiveAbilities::Lightning))
	{
		// 아직 진화 안함
		return;
	}

	// 0.5초 이후에 마지막 위치에 lightning 발사  
	FTimerHandle delayTimer;
	GetWorld()->GetTimerManager().SetTimer(delayTimer,FTimerDelegate::CreateLambda([&]()
	{
		ABase_Character* Character = IInterface_CharacterManager::Execute_GetCharacter(GetOwner());

		S_ExecuteLightning(LastLightningLoc,Character,CalculateBonusDamage(LightningDamage),LightningRadius);
	}),
	0.5f,
	false);	 
}

void UAbilitiesComponent::LevelUpFireball()
{
	switch (LevelUpActive(EActiveAbilities::Fireball))
	{
	case 1:
		GrantFireball(true);
		break;
	case 2:
	case 4:
		FireballDamage +=5.0f;
		break;
	case 3:
		FireballRadius *= 1.1f;
		break;
	case 5:
		FireballRadius *=1.1f;
		EvolutionMap.Add(EActiveAbilities::Fireball,EPassiveAbilities::Speed_Bonus);
		break;
	default:
		break;
	}
}

void UAbilitiesComponent::GrantFireball(bool Cast)
{
	FireBallDelegate.BindUFunction(this,FName("PrepareFireball"));
	if(!FireBallDelegate.IsBound())
	{
		RS_LOG_ERROR(TEXT("바운드 실패"))
	}
	FTimerHandle FireballTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireballTimerHandle,FireBallDelegate,/*CalculateTimerMode(FireballTimer)*/2.0f,true);

	ActiveTimers.AddUnique(FireballTimerHandle);

	if(Cast)
	{
		PrepareFireball();
	}
}

void UAbilitiesComponent::PrepareFireball()
{
	// IInterface 상속 검증 
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("User Character가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}
	// null이다? 왜?
	USphereComponent* AbilitySphere = IInterface_CharacterManager::Execute_GetAbilitySphere(GetOwner());

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_COLLISION_ENEMY);
	
	bool bAnyHit = GetWorld()->OverlapMultiByObjectType(OverlapResults,GetOwner()->GetActorLocation(),
		FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(AbilitySphere->GetScaledSphereRadius()));

	// 여기부터

	if(!bAnyHit)
	{
		// 없으면 함수 종료
		return;
	}

	TArray<AActor*> Actors;

	float distance;
	
	for(FOverlapResult overlap : OverlapResults)
	{
		Actors.AddUnique(overlap.GetActor());
	}
	// null이다. 왜?
	ABase_Character* Char = IInterface_CharacterManager::Execute_GetCharacter(GetOwner());

	AActor* LocNearestActor = UGameplayStatics::FindNearestActor(GetOwner()->GetActorLocation(),Actors,distance);

	S_ExecuteFireball_Implementation(LocNearestActor,Char,CalculateBonusDamage(FireballDamage),FireballRadius);

	if(!CheckEvoActive(EActiveAbilities::Fireball))
		return;
	
	if(!FireballFlipflop.Flip())
		return;
	
	//  0.5초 delay 후 PrepareFireball 호출 
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateLambda([&]()
	{
		PrepareFireball();
	}),
	0.5f,
	false
	);
}

void UAbilitiesComponent::S_ExecuteFireball_Implementation(AActor* Target, ABase_Character* Character, float Damage,
	float Radius)
{
	if(!IsValid(Target))
	{
		RS_LOG_ERROR(TEXT("Fireball의 Target이 유효하지 않습니다."))
		return;
	}

	FTransform FireballTransform;
	FireballTransform.SetIdentity();
	FireballTransform.SetLocation(Character->GetActorLocation());
	FireballTransform.SetRotation((Target->GetActorLocation() - Character->GetActorLocation()).Rotation().Quaternion());
	
	if(AFireball_Projectile* Fireball = GetWorld()->SpawnActorDeferred<AFireball_Projectile>(FireballClass,FireballTransform
		,nullptr,Character,ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		Fireball->Radius = Radius;
		Fireball->Damage = Damage;
		Fireball->SetInstigator(Character);
		Fireball->Hit_VFX = FireballEffect;
		Fireball->FinishSpawning(FireballTransform);
	}

	MC_Fireball_Implementation();
}

void UAbilitiesComponent::S_ExecuteLightning_Implementation(const FVector& TargetLocation, ABase_Character* Instigator,
                                                            float Damage, float Radius)
{
	FTransform LightningTransform;
	LightningTransform.SetIdentity();
	LightningTransform.SetTranslation(TargetLocation - FVector(0.0f,0.0f,100.0f));
	
	if(ALightningExplosion* Lightning = GetWorld()->SpawnActorDeferred<ALightningExplosion>(LightningClass,LightningTransform))
	{
		Lightning->Radius = Radius;
		// Lightning->DrawDebugType = EDrawDebugTrace::Type::None;
		Lightning->SetInstigator(Instigator);
		Lightning->FinishSpawning(LightningTransform);
	}
}

void UAbilitiesComponent::S_ExecuteFrostBolt_Implementation(AActor* Target, ABase_Character* Character, float Damage)
{
	if(!IsValid(Target))
	{
		RS_LOG_ERROR(TEXT("ExecuteFrostBolt의 Target이 유효하지 않습니다."))
		return;
	}

	FTransform transform;
	transform.SetIdentity();
	transform.SetLocation(Character->GetActorLocation());
	FVector direction = Target->GetActorLocation() - Character->GetActorLocation();
	FRotator rot =direction.Rotation();
	transform.SetRotation(rot.Quaternion());
	
	if(ABase_Projectile* projectile = GetWorld()->SpawnActorDeferred<ABase_Projectile>(BaseProjectileClass,transform,nullptr,Character,ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		projectile->Damage = Damage;

		projectile->FinishSpawning(transform);
	}

	MC_Frostbolt_Implementation();
}


void UAbilitiesComponent::MC_Frostbolt_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),FrostBoltSound,GetOwner()->GetActorLocation());
}

void UAbilitiesComponent::MC_Fireball_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),FireballSound,GetOwner()->GetActorLocation());
}

void UAbilitiesComponent::LevelUpMaxHealth(bool PowerUp)
{
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("Owner가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}
	
	if(!PowerUp)
	{
		switch (LevelUpPassive(EPassiveAbilities::Health_Bonus))
		{
		case 1:
		case 2:
		case 3:
		case 4:
			IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Health_Bonus,1.1f);
			break;
		case 5:
			IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Health_Bonus,1.1f);
			EvolutionPassiveArray.AddUnique(EPassiveAbilities::Health_Bonus);
			break;
		default:
			break;
		}
	}
	else
	{
		IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Health_Bonus,1.1f);
	}
}

void UAbilitiesComponent::LevelUpTimerReduction(bool PowerUp)
{
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("Owner가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}
	if(!PowerUp)
	{
		switch (LevelUpPassive(EPassiveAbilities::Ability_Cooldown_Reduction))
		{
		case 1:
		case 2:
		case 3:
		case 4:
			AbilityTimerMultiplier *= 0.95f;
			break;
		case 5:
			AbilityTimerMultiplier *= 0.95f;
			EvolutionPassiveArray.AddUnique(EPassiveAbilities::Ability_Cooldown_Reduction);
			break;
		default:
			break;
		}
	}
	else
	{
			AbilityTimerMultiplier *= 0.95f;
	}
}

void UAbilitiesComponent::LevelUpAbilityDamageBonus(bool PowerUp)
{
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("Owner가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}
	if(!PowerUp)
	{
		switch (LevelUpPassive(EPassiveAbilities::Ability_Bonus_Damage))
		{
		case 1:
		case 2:
		case 3:
		case 4:
			AbilityDamageMultiplier *= 1.1f;
			break;
		case 5:
			AbilityDamageMultiplier *= 1.1f;
			EvolutionPassiveArray.AddUnique(EPassiveAbilities::Ability_Bonus_Damage);
			break;
		default:
			break;
		}
	}
	else
	{
		AbilityDamageMultiplier *= 1.1f;
	}
}

void UAbilitiesComponent::LevelUpSpeedBonus(bool PowerUp)
{
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		RS_LOG_ERROR(TEXT("Owner가 IInterface_CharacterManager를 상속받지 않았습니다."))
		return;
	}
	if(!PowerUp)
	{
		switch (LevelUpPassive(EPassiveAbilities::Speed_Bonus))
		{
		case 1:
		case 2:
		case 3:
		case 4:
			IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Speed_Bonus,1.1f);
			break;
		case 5:
			IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Speed_Bonus,1.1f);
			EvolutionPassiveArray.AddUnique(EPassiveAbilities::Speed_Bonus);
			break;
		default:
			break;
		}
	}
	else
	{
		IInterface_CharacterManager::Execute_AdjustPassive(GetOwner(),EPassiveAbilities::Speed_Bonus,1.1f);
	}
}

void UAbilitiesComponent::RefreshAbilities()
{
	ActiveTimers.Empty();

	TArray<EActiveAbilities> ActiveAbilities;
	ActiveAbilitiesMap.GetKeys(ActiveAbilities);

	for(EActiveAbilities active : ActiveAbilities)
	{
		switch (active)
		{
		case EActiveAbilities::Hammer:
			GrantHammer(false);
			break;
		case EActiveAbilities::FrostBolt:
			GrantFrostBolt(false);
			break;
		case EActiveAbilities::Lightning:
			GrantLightning(false);
			break;
		case EActiveAbilities::Fireball:
			GrantFireball(false);
			break;
		default:
			break;
		}
	}
}

void UAbilitiesComponent::InvalidateTimers()
{
	for(FTimerHandle& TimerHandle : ActiveTimers)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UAbilitiesComponent::SetStartingAbility()
{
	USG_Player* SGPlayer = UFunctionLibrary_Helper::LoadPlayerData(GetWorld());

	switch (SGPlayer->Character.StartingAbilities)
	{
	case EActiveAbilities::Hammer:
		LevelUpHammer();
		break;
	case EActiveAbilities::FrostBolt:
		LevelUpFrostBolt();
		break;
	case EActiveAbilities::Lightning:
		LevelUpLightning();
		break;
	case EActiveAbilities::Fireball:
		LevelUpFireball();
		break;
	}
}

int32 UAbilitiesComponent::LevelUpActive(EActiveAbilities Active)
{
	int32 ActiveAbilityLevel = 1;
	if(ActiveAbilitiesMap.Contains(Active))
	{
		ActiveAbilityLevel = *ActiveAbilitiesMap.Find(Active) + 1;
	}
	
	ActiveAbilitiesMap.Add(Active,ActiveAbilityLevel);

	return ActiveAbilityLevel;
}

int32 UAbilitiesComponent::LevelUpPassive(EPassiveAbilities Passive)
{
	int32 PassiveAbilityLevel = 1;
	if(PassiveAbilitiesMap.Contains(Passive))
	{
		PassiveAbilityLevel = *PassiveAbilitiesMap.Find(Passive) + 1;
	}
	
	PassiveAbilitiesMap.Add(Passive,PassiveAbilityLevel);

	return PassiveAbilityLevel;
}

float UAbilitiesComponent::CalcAbilityDamageWithCrit(float weight, EActiveAbilities ActiveAbility, float BaseDamage)
{
	float CalculatedDmg = CalculateBonusDamage(BaseDamage);

	if(!CheckEvoActive(ActiveAbility))
	{
		return CalculatedDmg;
	}
	
	// 크리 터지면 2배

	return weight>=FMath::FRandRange(0.0f,1.0f)? 2.0f*CalculatedDmg:CalculatedDmg;
}

void UAbilitiesComponent::PauseAbilities()
{
	for(const FTimerHandle& AbilityTimers : ActiveTimers)
	{
		if(AbilityTimers.IsValid())
			GetWorld()->GetTimerManager().PauseTimer(AbilityTimers);
	}
}

void UAbilitiesComponent::UnPauseAbilities()
{
	for(const FTimerHandle& AbilityTimers : ActiveTimers)
	{
		if(AbilityTimers.IsValid())
			GetWorld()->GetTimerManager().UnPauseTimer(AbilityTimers);
	}
}

void UAbilitiesComponent::MC_Hammer_Implementation(float Radius)
{
	UGameplayStatics::SpawnEmitterAttached(HammerEffect,GetOwner()->GetRootComponent(),NAME_None,FVector(),FRotator(),
		FVector(Radius/100.0f,Radius/100.0f,1.5f) );

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),HammerSound,GetOwner()->GetActorLocation());
}





