// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base_Character.h"

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Data/Attribute/CombatSet.h"
#include "Data/Attribute/HealthSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayActors/FloatingTextActor.h"
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
#include "UI/UW_HUDHealthBar.h"

#include "GameplayAbilitiesModule.h"
#include "AbilitySystemGlobals.h"
#include "Components/TextBlock.h"
#include "Data/Attribute/ExpSet.h"
#include "UI/UW_PlayerHud.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Components/RsWeaponComponent.h"
#include "Data/Attribute/CrosshairAttSet.h"
#include "System/RsHUD.h"
#include "System/RsPlayerState.h"
#include "System/UnitWidgetController.h"
#include "GameplayAbility/GA_Skill.h"

// Sets default values
ABase_Character::ABase_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	DeathDoOnce.Reset();



	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetLineThickness(0.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);

	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	
	GetMesh()->SetRelativeLocation(FVector(0.0f,0.0f,-90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f,270.0f,0.0f));


	
	// InventoryComponent 세팅
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	InventoryComponent->bEditableWhenInherited = true;
	
	StartLevel = 1;
	
	WeaponComponent = CreateDefaultSubobject<URsWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComponent->bEditableWhenInherited =  true;
}

// Called when the game starts or when spawned
void ABase_Character::BeginPlay()
{
	Super::BeginPlay();

	SetupReference();

	LoadLastCharacterClass();
	
	CharacterInputSetting();
}

// Called every frame
void ABase_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AbilitySystemComponent)
	{
		// 1. AttributeSet에서 기본 상태(최소 탄퍼짐)와 회복 속도를 읽어옵니다.
		float MinSpread = AbilitySystemComponent->GetNumericAttribute(UCrosshairAttSet::GetSpreadMinAttribute());
		float RecoveryRate = AbilitySystemComponent->GetNumericAttribute(UCrosshairAttSet::GetSpreadRecoveryRateAttribute());

		// 2. 현재 탄퍼짐이 기본 상태보다 넓게 벌어져 있다면?
		if (CurrentSpread > MinSpread)
		{
			// 🌟 [핵심] FInterpTo를 사용해 부드럽게 좁혀줍니다.
			// (현재값, 목표값, DeltaTime, 회복속도)
			CurrentSpread = FMath::FInterpTo(CurrentSpread, MinSpread, DeltaTime, RecoveryRate);
		}
	}
	
}

void ABase_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void ABase_Character::OnRep_Controller()
{
	Super::OnRep_Controller();
}


void ABase_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	ARsPlayerState* PS = Cast<ARsPlayerState>(GetPlayerState());
	if (PS)
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
	else
	{
		RS_LOG_WARNING(TEXT("Failed to get PlayerState"))
	}
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}

void ABase_Character::AddCharacterAbilities()
{
	// 1. 서버 권한 확인 (중요: 클라이언트에서 실행하면 안 됨)
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
	{
		return;
	}

	// 2. 어빌리티 순회하며 부여
	for (TSubclassOf<UGameplayAbility> GA : DefaultAbilities)
	{
		if (GA)
		{
			// 3. Spec 생성 (클래스, 레벨, 입력ID, 소스)
			// 예시: 레벨 1, 입력 ID는 -1 (없음) 또는 Enum 값
			FGameplayAbilitySpec Spec(GA, 1, -1, this);

			// 4. 어빌리티 부여 (GiveAbility)
			// 리턴받은 Handle은 나중에 필요하면 저장해둡니다.
			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		}
	}
	
	for (const FSkillInputMapping& Mapping : DefaultSkills)
	{
		if (Mapping.Ability)
		{
			int32 InputID = Mapping.InputAction?(int32)Mapping.InputID:-1;
			
			// 3. Spec 생성 (클래스, 레벨, 입력ID, 소스)
			// 예시: 레벨 1, 입력 ID는 -1 (없음) 또는 Enum 값
			FGameplayAbilitySpec Spec(Mapping.Ability, 1, InputID, this);

			// 4. 어빌리티 부여 (GiveAbility)
			// 리턴받은 Handle은 나중에 필요하면 저장해둡니다.
			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}


UAbilitySystemComponent* ABase_Character::GetAbilitySystemComponent() const
{
	if (AbilitySystemComponent) 
		return AbilitySystemComponent;
	
	if (ARsPlayerState* PS = GetPlayerState<ARsPlayerState>())
	{
		return PS->GetAbilitySystemComponent();
	}
	return nullptr;
}

void ABase_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ARsPlayerState* PS = Cast<ARsPlayerState>(GetPlayerState());
	if (PS)
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
	else
	{
		RS_LOG_WARNING(TEXT("Failed to get PlayerState"))
	}
	// [중요] 서버 쪽 초기화: 여기서 Init을 해줘야 ASC가 작동을 시작합니다.
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
		
		// InitializeDefaultAttrbute();
		
		AbilitySystemComponent->InitAbilityActorInfo(PS,this);
		
		// 초기 스킬 및 스탯 부여 
		AddCharacterAbilities();
		
		// 0204 InitAbilityActorInfo 이후에 호출, 그리고 Attribute값들이 추가된 이후에 호출 
		// 이걸 그냥 ge를 생성해서 따로 적용하자
		// InitAttributeDefaults_ServerOnly();
		ApplyAttributeOnLevel(StartLevel);
		// OnLevelup(StartLevel);
	}
}

void ABase_Character::OnLevelup( float NewLevel)
{
	// 서버에서만 적용 
	if (!HasAuthority()) return;
	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultCurveEffectClass,NewLevel,Context);
	
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}






float ABase_Character::GetMaxXpForLevel(float pLevel) const
{
	float ResultXp = 100.0f;
	if (MaxXpCurve.CurveTable)
	{
		ResultXp = MaxXpCurve.CurveTable->FindCurve(MaxXpCurve.RowName,TEXT(""))->Eval(pLevel);
		// RS_LOG_SCREEN(TEXT("max : %f"), ResultXp)
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
			
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABase_Character::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ABase_Character::Move);
		}
		
		for (const FSkillInputMapping& Mapping : DefaultSkills)
		{
			if (!Mapping.InputAction || !Mapping.Ability) continue;
			
			// 🌟 [핵심 기법] CDO (Class Default Object) 가져오기!
			// 스킬을 스폰하지 않고도, 해당 블루프린트의 기본 세팅값을 미리 읽어옵니다.
			UGA_Skill* DefaultSkill = Mapping.Ability->GetDefaultObject<UGA_Skill>();

			// 스킬 타입에 따라 알아서 바인딩을 분기 처리합니다.
			if (DefaultSkill->GetSkillInputStyle() == ESkillInputStyle::Continuous)
			{
				// 연사형: 버튼을 누르고 있는 동안 매 프레임 스킬 실행 시도
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Triggered, this, &ABase_Character::SendAbilityLocalInput,Mapping.InputID,true);
			}
			else 
			{
				// 단발형/차징형: 눌렀을 때 실행, 뗐을 때 종료/취소
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Started, this, &ABase_Character::SendAbilityLocalInput, Mapping.InputID,true);
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &ABase_Character::SendAbilityLocalInput, Mapping.InputID,false);
			}
		}
		
	// 	// 지금은 하드코딩으로 넣어주지만 
	// 	// 스킬마다 넣어주는 것은 어려우니
	// 	// 데이터 애셋을 만들어서 클릭으로 동작하는지 홀드로 동작하는지, InputID는 무엇인지 담아 데이터를 가져오도록 하자.
	// 	if (Skill1Action)
	// 	{
	// 		// TODO :   임시로 잠깐만
	// 		EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Triggered,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill1,true);
	// 		
	// 		
	// 	// 	EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill1,true);
	// 	// 	
	// 	// 	EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill1,false);
	// 	 }
	// 	
	// 	if (Skill2Action)
	// 	{
	// 		EnhancedInputComponent->BindAction(Skill2Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill2,true);
	// 		
	// 		EnhancedInputComponent->BindAction(Skill2Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill2,false);
	// 	}
	// 	
	// 	if (Skill3Action)
	// 	{
	// 		EnhancedInputComponent->BindAction(Skill3Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill3,true);
	// 		
	// 		EnhancedInputComponent->BindAction(Skill3Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill3,false);
	// 	}
	// 	
	// 	if (Skill4Action)
	// 	{
	// 		EnhancedInputComponent->BindAction(Skill4Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill4,true);
	// 		
	// 		EnhancedInputComponent->BindAction(Skill4Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill4,false);
	// 	}
	// 	
	// 	if (Skill5Action)
	// 	{
	// 		EnhancedInputComponent->BindAction(Skill5Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill5,true);
	// 		
	// 		EnhancedInputComponent->BindAction(Skill5Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill5,false);
	// 	}
	}
}

void ABase_Character::AddSpread()
{
	if (!AbilitySystemComponent) return;

	// 1. AttributeSet에서 증가량과 최댓값을 읽어옵니다.
	float SpreadInc = AbilitySystemComponent->GetNumericAttribute(UCrosshairAttSet::GetSpreadIncreasePerShotAttribute());
	float MaxSpread = AbilitySystemComponent->GetNumericAttribute(UCrosshairAttSet::GetSpreadMaxAttribute());

	// 2. 현재 탄퍼짐에 증가량을 더하되, 최댓값(MaxSpread)을 넘지 않도록 Clamp(제한) 합니다.
	CurrentSpread = FMath::Min(CurrentSpread + SpreadInc, MaxSpread);
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
}






void ABase_Character::Die(AActor* DamageCauser)
{
	Super::Die(DamageCauser);
	
	
	IsDead = true;

	Death();

	MC_Death();

	if (ABase_GameMode* GM = Cast<ABase_GameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->DetermineGameStatus();
	}
}







void ABase_Character::Death_Implementation()
{
	DisableInput(LocalPlayerController);
}


void ABase_Character::MC_Death_Implementation()
{
	PlayAnimMontage(DeathMontage);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_ENEMY,ECR_Ignore);

	GetMesh()->SetCollisionResponseToChannel(ECC_COLLISION_PROJECTILE,ECR_Ignore);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
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


}

void ABase_Character::OROnRepCharacterClass()
{
}

void ABase_Character::OnRep_Character()
{
	OROnRepCharacterClass();
	
	// StartingAbility = Character.StartingAbilities;

	// S_SetCharacterMesh(Character.CharacterSK);
}

void ABase_Character::OnRep_CharSK()
{
	// if(IsValid(CharSK))
	// {
	// 	GetMesh()->SetSkinnedAssetAndUpdate(CharSK,false);
	// }
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

	CachedMoveInput = MovementVector;
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

void ABase_Character::SendAbilityLocalInput(const EAbilityInputID InputID, bool bIsPressed)
{
	if (GetAbilitySystemComponent())
	{
		if (bIsPressed)
		{
			// ASC에게 "이 ID 눌렸어"라고 알림 -> 자동으로 연결된 GA 발동 시도
			GetAbilitySystemComponent()->AbilityLocalInputPressed((int32)InputID);
		}
		else
		{
			// ASC에게 "이 ID 떼졌어"라고 알림 -> GA 내부에서 WaitInputRelease 태스크가 반응함
			GetAbilitySystemComponent()->AbilityLocalInputReleased((int32)InputID);
		}
	}
}




