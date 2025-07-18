// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitiesComponent.h"

#include "Net/UnrealNetwork.h"
#include "RogueShooter/RSEnumStruct.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/RSCollisionChannel.h"
#include "Library/FunctionLibrary_Helper.h"
#include "RogueShooter/AssetPath.h"


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
	int32 HammerLevel = 1;
	if(ActiveAbilitiesMap.Contains(EActiveAbilities::Hammer))
	{
		HammerLevel = *ActiveAbilitiesMap.Find(EActiveAbilities::Hammer) + 1;
	}
	
	ActiveAbilitiesMap.Add(EActiveAbilities::Hammer,HammerLevel);

	switch(HammerLevel)
	{
	case 1:
		GrantHammer(true);
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

void UAbilitiesComponent::S_ExecuteHammer_Implementation(TArray<FHitResult> Hits, float Damage, float Radius,
	APlayerController* Controller)
{
	MC_Hammer_Implementation(Radius);

	UFunctionLibrary_Helper::DamageEnemiesOnce(this,Hits,Damage,Controller,GetOwner());
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





