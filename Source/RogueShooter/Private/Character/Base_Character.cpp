// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base_Character.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AbilitiesComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/Interface_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "Net/UnrealNetwork.h"
#include "RogueShooter/AssetPath.h"
#include "Saves/SG_Player.h"
#include "System/Base_GameMode.h"
#include "System/Gameplay_PlayerController.h"
#include "Utility/RSCollisionChannel.h"
#include "Utility/RSLog.h"
#include "UI/UW_HealthBar.h"


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

	// TODO : healthbar 경로 추가 
	static ConstructorHelpers::FClassFinder<UUW_HealthBar> HealthbarClassFinder(*AssetPath::Blueprint::WBP_HealthBar_C);
	if(HealthbarClassFinder.Succeeded())
	{
		HealthBarClass = HealthbarClassFinder.Class;
	}
	else
	{
		RS_LOG_ERROR(TEXT("HealthBarClass를 찾을 수 없습니다."))
	}

	// HealthWidget  초기화 
	{
		HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");

		HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

		HealthWidget->SetDrawSize(FVector2D(125.0f,18.0f));

		HealthWidget->SetPivot(FVector2D(0.5f,0.5f));
	
		HealthWidget->SetRelativeLocation(FVector(0.0f,0.0f,125.0f));

		HealthWidget->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	SetupReference();

	LoadLastCharacterClass();

	FTimerHandle BeginTimer;
	GetWorld()->GetTimerManager().SetTimer(BeginTimer,FTimerDelegate::CreateLambda([this]()
	{
		OC_SetupWidgets();
	}),
	1.0f,
	false
	);
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

	S_SetCharacterData(AvailableCharacter);
}

void ABase_Character::S_SetCharacterMesh_Implementation(USkeletalMesh* SK)
{
	CharSK = SK;
}


void ABase_Character::S_SetCharacterData_Implementation(FAvailableCharacter CharacterData)
{
	Character = CharacterData;
}

void ABase_Character::CreateHealthWidget()
{
	UUW_HealthBar* HealthBar = Cast<UUW_HealthBar>(CreateWidget(GetPlayerState()->GetPlayerController(),HealthBarClass));

	HealthBarWidgetReference = HealthBar;

	HealthWidget->SetWidget(HealthBarWidgetReference);

	MC_UpdateHealthBar(CurrentHealth/MaxHealth);
}

void ABase_Character::SetupReference()
{
	if(AGameplay_PlayerController* Gameplay_PlayerController = Cast<AGameplay_PlayerController>(GetController()))
	{
		LocalPlayerController = Gameplay_PlayerController;
	}
}

void ABase_Character::LoadLastCharacterClass()
{
	USG_Player* SavedPlayer = UFunctionLibrary_Helper::LoadPlayerData(GetWorld());

	GameSave = SavedPlayer;

	S_SetCharacterData(GameSave->Character);
}

void ABase_Character::SetupDispatchers()
{
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

			if(!GM_Interface.GetClass()->ImplementsInterface(UInterface_GameManager::StaticClass()))
			{
				RS_LOG_ERROR(TEXT("GM_Interface 변수가 IInterface_GameManager를 상속받지 않았습니다."))
			}
			else
			{
				IInterface_GameManager::Execute_OnPlayerDeath(GM_Interface);
				
				AbilityComponent->InvalidateTimers();
			}
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABase_Character::Death_Implementation()
{
	AbilityComponent->InvalidateTimers();

	DisableInput(LocalPlayerController);
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

void ABase_Character::SetupHealthWidget_Implementation()
{
	// IInterface_CharacterManager::SetupHealthWidget_Implementation();
	OC_SetupWidgets();
}

void ABase_Character::OC_SetupWidgets_Implementation()
{
	CreateHealthWidget();
}

void ABase_Character::S_RestoreHealth_Implementation(float amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth+amount,0.0f,MaxHealth);

	MC_UpdateHealthBar(CurrentHealth/MaxHealth);
}

void ABase_Character::MC_UpdateHealthBar_Implementation(float percent)
{
	if(IsValid(HealthBarWidgetReference))
	{
		HealthBarWidgetReference->ProgressBar->SetPercent(percent);
	}
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

void ABase_Character::OROnRepCharacterClass()
{
}

void ABase_Character::OnRep_Character()
{
	OROnRepCharacterClass();
	
	StartingAbility = Character.StartingAbilities;

	S_SetCharacterMesh(Character.CharacterSK);
}

void ABase_Character::OnRep_CharSK()
{
	if(IsValid(CharSK))
	{
		GetMesh()->SetSkinnedAssetAndUpdate(CharSK,false);
	}
}

void ABase_Character::S_UpdatePassiveStat_Implementation(EPassiveAbilities Stat, float Value)
{
	switch (Stat)
	{
	case EPassiveAbilities::Health_Bonus:
		MaxHealth = MaxHealth*Value;
		IInterface_CharacterManager::Execute_RestoreHealth(this,MaxHealth*0.1f);
		break;
	case EPassiveAbilities::Speed_Bonus:
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * Value;
		break;
	default:
		break;
	}
}



