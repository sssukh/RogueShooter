// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilitiesComponent.h"

#include "Net/UnrealNetwork.h"
#include "RogueShooter/RSEnumStruct.h"
#include "DrawDebugHelpers.h"
#include "Abilities/Base_Projectile.h"
#include "Engine/OverlapResult.h"
#include "Interface/Interface_CharacterManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/RSCollisionChannel.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"


// Sets default values for this component's properties
UAbilitiesComponent::UAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
	
	// ...

	ConstructorHelpers::FObjectFinder<UParticleSystem> HammerEmitterFinder(*AssetPath::EffectTemplate::HammerEffect);
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
}


// Called when the game starts
void UAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HammerDelegate.BindUFunction(this,FName("PrepareHammer"));
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
	return EvolutionTracker[(int32)InEnumActive];
}

void UAbilitiesComponent::LevelUpHammer()
{
	int32 HammerLevel = LevelUpAbility(EActiveAbilities::Hammer);

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

void UAbilitiesComponent::LevelUpFrostBolt()
{
	switch(LevelUpAbility(EActiveAbilities::Frost_Bolt))
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
		EvolutionMap.Add(EActiveAbilities::Frost_Bolt,EPassiveAbilities::Health_Bonus);
		break;
	default:
		break;
	}

	
}

void UAbilitiesComponent::GrantFrostBolt(bool Cast)
{
	FTimerHandle FrostTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FrostTimerHandle,FrostBoltDelegate,CalculateTimerMode(FBTimer),true);
	ActiveTimers.AddUnique(FrostTimerHandle);

	if(Cast)
	{
		Prepare_FrostBolt();
	}
}

void UAbilitiesComponent::Prepare_FrostBolt()
{
	FBFireIndex = 0;

	// IInterface 상속 검증 
	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
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

	if(!GetOwner()->GetClass()->ImplementsInterface(UInterface_CharacterManager::StaticClass()))
	{
		// Character Manager 상속 안받았으면 함수 종료(에러)
		return;
	}

	TArray<AActor*> Actors;

	float distance;
	
	for(FOverlapResult overlap : OverlapResults)
	{
		Actors.AddUnique(overlap.GetActor());
	}
	
	ABase_Character* Char = IInterface_CharacterManager::Execute_GetCharacter(GetOwner());

	AActor* NearestActor = UGameplayStatics::FindNearestActor(GetOwner()->GetActorLocation(),Actors,distance);

	S_ExecuteFrostBolt_Implementation(NearestActor,Char,CalculateBonusDamage(FBDamage));

	// ExecuteFrostBolt 호출 후 0.05초 delay
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateLambda([this,&]()
	{
		if(FBFireIndex<=FBFireCount)
		{
			++FBFireIndex;
			S_ExecuteFrostBolt_Implementation(NearestActor,Char,CalculateBonusDamage(FBDamage));
		}
	}),
	0.05f,
	true
	);
}

void UAbilitiesComponent::S_ExecuteFrostBolt_Implementation(AActor* Target, ABase_Character* Character, float Damage)
{
	if(!IsValid(Target))
	{
		RS_LOG_ERROR(TEXT("ExecuteFrostBolt의 Target이 유효하지 않습니다."))
		return;
	}

	FTransform transform;
	transform.SetIdentityZeroScale();
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

void UAbilitiesComponent::S_ExecuteHammer_Implementation(TArray<FHitResult> Hits, float Damage, float Radius,
                                                         APlayerController* Controller)
{
	MC_Hammer_Implementation(Radius);

	UFunctionLibrary_Helper::DamageEnemiesOnce(this,Hits,Damage,Controller,GetOwner());
}

void UAbilitiesComponent::MC_Frostbolt_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),FrostBoltSound,GetOwner()->GetActorLocation());
}

int32 UAbilitiesComponent::LevelUpAbility(EActiveAbilities Active)
{
	int32 ActiveAbilityLevel = 1;
	if(ActiveAbilitiesMap.Contains(Active))
	{
		ActiveAbilityLevel = *ActiveAbilitiesMap.Find(Active) + 1;
	}
	
	ActiveAbilitiesMap.Add(Active,ActiveAbilityLevel);

	return ActiveAbilityLevel;
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

void UAbilitiesComponent::MC_Hammer_Implementation(float Radius)
{
	UGameplayStatics::SpawnEmitterAttached(HammerEffect,GetOwner()->GetRootComponent(),NAME_None,FVector(),FRotator(),
		FVector(Radius/100.0f,Radius/100.0f,1.5f) );

	UGameplayStatics::PlaySoundAtLocation(GetWorld(),HammerSound,GetOwner()->GetActorLocation());
}





