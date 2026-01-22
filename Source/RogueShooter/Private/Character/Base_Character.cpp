// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base_Character.h"

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Data/CombatSet.h"
#include "Data/HealthSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayActors/FloatingTextActor.h"
#include "Interface/Interface_GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Library/FunctionLibrary_Helper.h"
#include "Net/UnrealNetwork.h"
#include "RogueShooter/AssetPath.h"
#include "Saves/SG_Player.h"
#include "System/Base_GameMode.h"
#include "System/Gameplay_PlayerController.h"
#include "UI/UW_AbilityTile.h"
#include "Utility/RSCollisionChannel.h"
#include "Utility/RSLog.h"
#include "UI/UW_HealthBar.h"

#include "GameplayAbilitiesModule.h"
#include "AbilitySystemGlobals.h"
#include "Components/TextBlock.h"
#include "Data/ExpSet.h"
#include "UI/UW_PlayerHud.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

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

	// TODO : 삭제
	
	// AbilitySphere = CreateDefaultSubobject<USphereComponent>("AbilitySphere");
	// AbilitySphere->SetSphereRadius(960.0f);
	// AbilitySphere->SetLineThickness(0.0f);
	// AbilitySphere->SetupAttachment(GetCapsuleComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	// SpringArm->TargetArmLength = 2500.0f;
	// SpringArm->SetRelativeRotation(FRotator(-35.0f,0.0f,0.0f));
	// SpringArm->bInheritPitch = false;
	// SpringArm->bInheritRoll = false;
	// SpringArm->bInheritYaw = false;
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	// Camera->SetFieldOfView(45.0f);
	// Camera->SetProjectionMode(ECameraProjectionMode::Perspective);
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceFinder(*AssetPath::Animation::BaseCharAnim);
	if(AnimInstanceFinder.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceFinder.Class);
	}
	
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,270.0f,0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(*AssetPath::Mesh::SKM_Quinn_Simple);

	if(MeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshFinder.Object);
	}

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

	
	
	// InventoryComponent 세팅
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	InventoryComponent->bEditableWhenInherited = true;
	
	// AbilitySystemComponent 세팅
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthAttributes = CreateDefaultSubobject<UHealthSet>(TEXT("HeathAttributes"));
	CombatAttributes = CreateDefaultSubobject<UCombatSet>(TEXT("CombatAttributes"));
	ExpAttributes = CreateDefaultSubobject<UExpSet>(TEXT("ExpAttributes"));
	
	// ASC에 AttributeSet 등록
	AbilitySystemComponent->AddAttributeSetSubobject<UHealthSet>(HealthAttributes);
	AbilitySystemComponent->AddAttributeSetSubobject<UCombatSet>(CombatAttributes);
	AbilitySystemComponent->AddAttributeSetSubobject<UExpSet>(ExpAttributes);
	
	StartLevel = 1;
}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	SetupReference();

	LoadLastCharacterClass();
	
	CharacterInputSetting();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		
		AddCharacterAbilities();
	
		HealthAttributes->OnHealthDamaged.AddDynamic(this,&ABase_Character::SpawnFloatingText);
		HealthAttributes->OnShieldDamaged.AddDynamic(this,&ABase_Character::SpawnFloatingText);
		ExpAttributes->OnLevelUp.AddDynamic(this,&ABase_Character::OnLevelup);
		ExpAttributes->OnExpChange.AddDynamic(this,&ABase_Character::OnExpChange);
		
		OnLevelup(StartLevel);
	}
	
	// 위젯 세팅
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

void ABase_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//TODO : 지금은 여기있지만 ASC를 PlayerState로 옮기고 이 부분도 옮겨야함
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(AbilitySystemComponent,TEXT("BaseCharacter"),StartLevel,true);
	
}

void ABase_Character::AddCharacterAbilities()
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

UAbilitySystemComponent* ABase_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABase_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// [중요] 서버 쪽 초기화: 여기서 Init을 해줘야 ASC가 작동을 시작합니다.
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ABase_Character::OnLevelup( float NewLevel)
{
	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultCurveEffectClass,NewLevel,Context);
	
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	MC_UpdateCurrentHealth(HealthAttributes->GetHealth());
	
	// TODO : 레벨 UI 갱신 필요 
	// TODO : 임시로 갱신 시킴
	AGameplay_PlayerController* PC = Cast<AGameplay_PlayerController>(GetController());
	
	float maxXp = ExpAttributes->GetMaxExpGained();
	float currentXp = ExpAttributes->GetExpGained();
	
	PC->UpdateLevelUI((int32)NewLevel);
}

void ABase_Character::OnExpChange(float NewExp)
{
	AGameplay_PlayerController* PC = Cast<AGameplay_PlayerController>(GetController());
	
	float maxXp = ExpAttributes->GetMaxExpGained();
	
	PC->UpdateExpBar(NewExp/maxXp);
}

float ABase_Character::GetMaxXpForLevel(float pLevel) const
{
	float ResultXp = 100.0f;
	if (MaxXpCurve.CurveTable)
	{
		ResultXp = MaxXpCurve.CurveTable->FindCurve(MaxXpCurve.RowName,TEXT(""))->Eval(pLevel);
	}
	
	return ResultXp;
}

// Called to bind functionality to input
void ABase_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent == nullptr)
	{
		RS_LOG_SCREEN(TEXT("PlayerInputComponent is null"))
		return;
	}
	
	// Enhanced Input Component로 캐스팅
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        
		// LookAction이 유효한지 확인 후 바인딩
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABase_Character::Look);
		}
		
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ABase_Character::Move);
		}
	}
	
}

void ABase_Character::UpdateCharacterClass_Implementation(FAvailableCharacter AvailableCharacter)
{
	// IInterface_CharacterManager::UpdateCharacterClass_Implementation(Character);

	S_SetCharacterData(AvailableCharacter);
}

	// TODO : 삭제

// UAbilitiesComponent* ABase_Character::GetAbilityComponent_Implementation()
// {
// 	return AbilityComponent;
// }

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

	HealthBarWidgetReference->CurrentHp = HealthAttributes->GetHealth();
	HealthBarWidgetReference->MaxHp = HealthAttributes->GetMaxHealth();
	HealthBarWidgetReference->RefreshHpBar();
	// MC_UpdateHealthBar(CurrentHealth,MaxHealth);
	
	HealthAttributes->OnCurrentHealthChanged.AddDynamic(this,&ABase_Character::MC_UpdateCurrentHealth);
	HealthAttributes->OnMaxHealthChanged.AddDynamic(this,&ABase_Character::MC_UpdateMaxHealth);
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
	if(USG_Player* SavedPlayer = UFunctionLibrary_Helper::LoadPlayerData(GetWorld()))
		GameSave = SavedPlayer;
	else
	{
		RS_LOG_ERROR(TEXT("Saved Player가 존재하지 않습니다."))
	}
	S_SetCharacterData(GameSave->Character);
}

void ABase_Character::SetupDispatchers()
{
}

float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                  class AController* EventInstigator, AActor* DamageCauser)
{
	// CurrentHealth = CurrentHealth-DamageAmount;

	// TODO : AttributeSet을 이용해서 값이 변하면 델리게이트를 호출해 자동으로 업데이트하도록 함.
	// MC_UpdateHealthBar(CurrentHealth/MaxHealth);

	// if(CurrentHealth<=0)
	// {
	// 	// TODO : 지금은 코드로 하지만 GAS로 하게되면 거기로 옮겨야됨 
	// 	CharDie();
	// }
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


	// TODO : GAS로 옮겨진거 체크 후 삭제 

void ABase_Character::CharDie_Implementation(AActor* Causer)
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
				
			// AbilityComponent->InvalidateTimers();
		}
	}
}
	// TODO : 삭제
	// TODO : 큐로 옮긴거 확인하고 삭제 
void ABase_Character::SpawnFloatingText(float InDamage, EDamageReceiveType DamageType)
{
	
}


void ABase_Character::MC_UpdateMaxHealth_Implementation(float pNewMaxHp)
{
	if (!HealthBarWidgetReference)
	{
		RS_LOG_ERROR(TEXT("Hp Bar Widget is NULL"))
		return;
	}
	
	HealthBarWidgetReference->MaxHp = pNewMaxHp;
	HealthBarWidgetReference->RefreshHpBar();
}

void ABase_Character::MC_UpdateCurrentHealth_Implementation(float pNewCurrentHp)
{
	if (!HealthBarWidgetReference)
	{
		RS_LOG_ERROR(TEXT("Hp Bar Widget is NULL"))
		return;
	}
	
	HealthBarWidgetReference->CurrentHp = pNewCurrentHp;
	HealthBarWidgetReference->RefreshHpBar();
}


void ABase_Character::Death_Implementation()
{
	// TODO : 삭제
	
	// AbilityComponent->InvalidateTimers();

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



// 어디서 호출되는지?
// 안됐으면 위젯이 왜 있는지?
// BeginPlay에서 OC_SetupWidgets()를 호출한다.
// 이 함수가 왜 있는건가?
// 그냥 인터페이스 용으로 남겨둔건가
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
	// CurrentHealth = FMath::Clamp(CurrentHealth+amount,0.0f,MaxHealth);

	// TODO : AttributeSet을 이용해서 값이 변하면 델리게이트를 호출해 자동으로 업데이트하도록 함.
	// MC_UpdateHealthBar(CurrentHealth/MaxHealth);
}



// TODO : 블루프린트 식 pause가 구현되어 있다.
// TODO : C++식 Pause를 구현해서 대체하자.
void ABase_Character::S_Pause_Implementation(bool Pause, bool Override)
{
	if(Pause)
	{
		MC_Pause(Pause);

		// AbilityComponent->PauseAbilities();
		
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

		// AbilityComponent->UnPauseAbilities();
		
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
	// TODO : 삭제
	
	// IInterface_CharacterManager::AdjustPassive_Implementation(Stat, MultiplicationAmount);

	// S_UpdatePassiveStat(Stat,MultiplicationAmount);
}

bool ABase_Character::IsAlive_Implementation()
{
	return !IsDead;
}

ABase_Character* ABase_Character::GetCharacter_Implementation()
{
	return this;
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
	
	// StartingAbility = Character.StartingAbilities;

	S_SetCharacterMesh(Character.CharacterSK);
}

void ABase_Character::OnRep_CharSK()
{
	if(IsValid(CharSK))
	{
		GetMesh()->SetSkinnedAssetAndUpdate(CharSK,false);
	}
}

void ABase_Character::Look(const FInputActionValue& Value)
{
	// Axis2D 데이터 가져오기 (X, Y)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 마우스 X 이동 -> 캐릭터/카메라의 Yaw(좌우) 회전
		AddControllerYawInput(LookAxisVector.X);

		// 마우스 Y 이동 -> 캐릭터/카메라의 Pitch(상하) 회전
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABase_Character::Move(const FInputActionValue& Value)
{
	// 입력값 가져오기 (X: 앞뒤, Y: 좌우)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// A. 컨트롤러의 회전값 가져오기
		const FRotator Rotation = Controller->GetControlRotation();
        
		// B. Pitch(위아래)와 Roll(기울기)은 무시하고, Yaw(좌우)만 남김
		//    (캐릭터가 하늘로 날아가거나 땅으로 꺼지지 않고, 수평으로만 이동하게 하기 위함)
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// C. 회전값으로부터 앞(Forward) 방향과 오른쪽(Right) 방향 벡터 구하기
		//    (언리얼 내부 수학 함수 이용)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// D. 구한 방향으로 이동 입력 추가
		//    W/S 입력(X값)은 앞뒤 방향으로 적용
		AddMovementInput(ForwardDirection, MovementVector.X);
        
		//    D/A 입력(Y값)은 좌우 방향으로 적용
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ABase_Character::CharacterInputSetting()
{
	// 1. 현재 이 캐릭터를 조종하는 플레이어 컨트롤러를 가져옵니다.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// 2. 컨트롤러에서 Enhanced Input Local Player Subsystem을 가져옵니다.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 3. 서브시스템에 매핑 컨텍스트(IMC)를 추가합니다. (우선순위 0)
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// TODO : 삭제

// void ABase_Character::S_UpdatePassiveStat_Implementation(EPassiveAbilities Stat, float Value)
// {
// 	switch (Stat)
// 	{
// 	case EPassiveAbilities::Health_Bonus:
// 		MaxHealth = MaxHealth*Value;
// 		IInterface_CharacterManager::Execute_RestoreHealth(this,MaxHealth*0.1f);
// 		break;
// 	case EPassiveAbilities::Speed_Bonus:
// 		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * Value;
// 		break;
// 	default:
// 		break;
// 	}
// }



