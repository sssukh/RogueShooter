// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base_Character.h"

#include "Camera/CameraComponent.h"
#include "Chaos/SoftsSpring.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/Interface_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "RogueShooter/AssetPath.h"
#include "System/Base_GameMode.h"
#include "Utility/RSCollisionChannel.h"


// Sets default values
ABase_Character::ABase_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	DeathDoOnce.Reset();

	ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAMFinder(*AssetPath::Montage::PlayerDeath);
	if(DeathAMFinder.Succeeded())
	{
		DeathAnimMontage = DeathAMFinder.Object;
	}

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetLineThickness(0.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);

	AbilitySphere = CreateDefaultSubobject<USphereComponent>("AbilitySphere");
	AbilitySphere->SetSphereRadius(960.0f);
	AbilitySphere->SetLineThickness(0.0f);
	AbilitySphere->SetupAttachment(GetCapsuleComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->TargetArmLength = 2500.0f;
	SpringArm->SetRelativeRotation(FRotator(0.0f,-35.0f,0.0f));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetFieldOfView(45.0f);
	Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetupAttachment(SpringArm);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceFinder(*AssetPath::Animation::BaseCharAnim);
	if(AnimInstanceFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceFinder.Class);
	}
	
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,270.0f,0.0f));
	
}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABase_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void ABase_Character::UpdateCharacterClass_Implementation(FAvailableCharacter AvailableCharacter)
{
	// IInterface_CharacterManager::UpdateCharacterClass_Implementation(Character);

	S_SetCharacterData_Implementation(AvailableCharacter);
}

void ABase_Character::S_SetCharacterMesh_Implementation(USkeletalMesh* SK)
{
	CharSK = SK;
}


void ABase_Character::S_SetCharacterData_Implementation(FAvailableCharacter CharacterData)
{
	Character = CharacterData;
}

void ABase_Character::SetupReference()
{
	// TODO : GameplayPlayerController 구현 필요 
	//	GetController()
}

float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                  class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = CurrentHealth-DamageAmount;

	MC_UpdateHealthBar(CurrentHealth/MaxHealth);

	if(CurrentHealth<=0)
	{
		if(DeathDoOnce.Execute())
		{
			IsDead = true;

			Death();

			MC_Death();

			if(GM_Interface.GetClass()->ImplementsInterface(UInterface_GameManager::StaticClass()))
			{
				IInterface_GameManager::Execute_OnPlayerDeath(GM_Interface);

				// TODO : AbilityComponent 변수 필요 
			}
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABase_Character::Death()
{
	// TODO : AbilityComponent 구현 필요
}

void ABase_Character::MC_Death_Implementation()
{
	PlayAnimMontage(DeathAnimMontage);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}



void ABase_Character::RestoreHealth_Implementation(float amount)
{
	// IInterface_CharacterManager::RestoreHealth_Implementation(amount);

	S_RestoreHealth(amount);
}

void ABase_Character::S_RestoreHealth_Implementation(float amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth+amount,0.0f,MaxHealth);

	MC_UpdateHealthBar(CurrentHealth/MaxHealth);
}

void ABase_Character::MC_UpdateHealthBar_Implementation(float percent)
{
}

void ABase_Character::S_Pause_Implementation(bool Pause, bool Override)
{
	if(Pause)
	{
		MC_Pause(Pause);

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.0001f);

		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

		ABase_GameMode* BaseGameMode = Cast<ABase_GameMode>(GameMode);

		BaseGameMode->GameIsPaused = true;
	}
	else
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

		ABase_GameMode* BaseGameMode = Cast<ABase_GameMode>(GameMode);
		if(Override)
		{
			BaseGameMode->ResetPauseCount();
		}
		else
		{
			if(!BaseGameMode->CheckPlayersForPause())
			{
				return;
			}
		}

		MC_Pause(Pause);

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0f);

		BaseGameMode->GameIsPaused = false;
	}
}


void ABase_Character::MC_Pause_Implementation(bool Pause)
{
	if(Pause)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.0001f);
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.0f);
	}
}



void ABase_Character::OC_Pause_Implementation(bool Pause, bool Override)
{
	S_Pause(Pause,Override);
}

void ABase_Character::Pause_Implementation(bool Pause, bool Override)
{
	// IInterface_CharacterManager::Pause_Implementation(Pause, Override);

	OC_Pause(Pause,Override);
}

void ABase_Character::AdjustPassive_Implementation(EPassiveAbilities Stat, float MultiplicationAmount)
{
	// IInterface_CharacterManager::AdjustPassive_Implementation(Stat, MultiplicationAmount);

	S_UpdatePassiveStat(Stat,MultiplicationAmount);
}

void ABase_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABase_Character,Character)

	DOREPLIFETIME(ABase_Character,CharSK);
}

void ABase_Character::OnRep_Character()
{
}

void ABase_Character::OnRep_CharSK()
{
}

void ABase_Character::S_UpdatePassiveStat_Implementation(EPassiveAbilities Stat, float Value)
{
	switch (Stat)
	{
	case EPassiveAbilities::Health_Bonus:
		MaxHealth = MaxHealth*Value;
		RestoreHealth_Implementation(MaxHealth*0.1f);
		break;
	case EPassiveAbilities::Speed_Bonus:
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * Value;
		break;
	default:
		break;
	}
}



