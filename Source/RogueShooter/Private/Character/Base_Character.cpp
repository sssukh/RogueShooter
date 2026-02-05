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
#include "System/RsHUD.h"
#include "System/UnitWidgetController.h"

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

	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
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
	
	
	
	StartLevel = 1;
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
}

void ABase_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 0204 : attribute 초기화를 서버에서만 하도록 수정
	// 이유 : 서버만 초기값을 계산하고 클라이언트는 서버가 보내준 값을 받아오기만 해야한다. 
	
	
	if (GetLocalRole() == ROLE_Authority)
	{
		// IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(AbilitySystemComponent,TEXT("BaseCharacter"),StartLevel,true);
		
		// TODO :  
		// broadcast InitialValues
		// OnLEvelup을 Level이 오를 때마다 호출시키기.
	}
	// BroadcastInitialValues();
}

void ABase_Character::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	// 클라이언트에서 위젯 초기화 
	InitHUDAndUI();
}


void ABase_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
        
		// 0204 InitAbilityActorInfo 호출하기전에 ASC에 AttributeSet들을 모두 추가 
		// InitializeDefaultAttrbute();
		
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	// 클라이언트에서 위젯 초기화 
	
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,FTimerDelegate::CreateLambda(
		[this]()
		{
			RS_LOG_WARNING(TEXT("Hp : %f, MaxHp : %f, Level : %f, Exp : %f")
				,HealthAttributes->GetHealth(),HealthAttributes->GetMaxHealth(),ExpAttributes->GetExpLevel(),ExpAttributes->GetExpGained())
			InitHUDAndUI();
		} ),0.2f,false);
}

void ABase_Character::AddCharacterAbilities()
{
	// 1. 서버 권한 확인 (중요: 클라이언트에서 실행하면 안 됨)
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent))
	{
		return;
	}

	// 2. 어빌리티 순회하며 부여
	for (const FGAbilityID& AbilityID : DefaultAbilities)
	{
		if (AbilityID.GameplayAbility)
		{
			int32 InputID = AbilityID.InputID==EAbilityInputID::None?-1:(int32)AbilityID.InputID;
			
			// 3. Spec 생성 (클래스, 레벨, 입력ID, 소스)
			// 예시: 레벨 1, 입력 ID는 -1 (없음) 또는 Enum 값
			FGameplayAbilitySpec Spec(AbilityID.GameplayAbility, 1, InputID, this);

			// 4. 어빌리티 부여 (GiveAbility)
			// 리턴받은 Handle은 나중에 필요하면 저장해둡니다.
			FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
			
		}
	}
}

void ABase_Character::InitializeDefaultAttrbute()
{
	// ASC에 AttributeSet 등록
	AbilitySystemComponent->AddAttributeSetSubobject<UHealthSet>(HealthAttributes);
	AbilitySystemComponent->AddAttributeSetSubobject<UCombatSet>(CombatAttributes);
	AbilitySystemComponent->AddAttributeSetSubobject<UExpSet>(ExpAttributes);
}

void ABase_Character::InitHUDAndUI()
{
	if (bIsGASInitialized)
		return;
	
	// Controller와 PlayerState 검사 
	if (GetController() == nullptr || GetPlayerState() == nullptr)
	{
		return;
	}
	
	if (IsLocallyControlled())
	{
		if (!AbilitySystemComponent || !HealthAttributes || !ExpAttributes || !CombatAttributes)
		{
			FTimerHandle TimerHandle_InitUI;
			GetWorldTimerManager().SetTimer(TimerHandle_InitUI, this, &ABase_Character::InitHUDAndUI, 0.1f, false);
			return;
		}
		
		if (AGameplay_PlayerController* PC = Cast<AGameplay_PlayerController>(GetController()))
		{
			if (ARsHUD* HUD = Cast<ARsHUD>(PC->GetHUD()))
			{
				HUD->InitOverlay(FWidgetControllerParams(PC,GetPlayerState(),AbilitySystemComponent));
				RS_LOG_SCREEN(TEXT("HUD Init Success!")) // 로그 확인용
			}
			else
			{
				RS_LOG_SCREEN(TEXT("HUD Casting Failed! Check GameMode HUD Class."))
			}
		
		
			if (HealthBarClass)
			{
				CreateHealthWidget(PC);
			}
		}
		else
		{
			RS_LOG_SCREEN( TEXT("PC Casting Failed! Check GameMode PlayerController Class."))
		}
		
		
			
		if (AbilitySystemComponent && HealthBarWidgetReference && CharacterWidgetControllerClass)
		{
			// 컨트롤러 인스턴스 생성 
			CharacterWidgetController = NewObject<UUnitWidgetController>(this,CharacterWidgetControllerClass);
		
			// 파라미터 주입
			FWidgetControllerParams Params;
			Params.AbilitySystemComponent = AbilitySystemComponent;
		
			CharacterWidgetController->SetWidgetControllerParams(Params);
			CharacterWidgetController->BindCallbacksToDependencies();
		
			// 위젯에 컨트롤러 연결 
			IInterface_WidgetManager::Execute_SetWidgetController(HealthBarWidgetReference,CharacterWidgetController);
		}
		
		
		OnLevelup(StartLevel);
		
		bIsGASInitialized = true;
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
		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
		
		// InitializeDefaultAttrbute();
		
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		
		// 초기 스킬 및 스탯 부여 
		AddCharacterAbilities();
		
		// 0204 InitAbilityActorInfo 이후에 호출, 그리고 Attribute값들이 추가된 이후에 호출 
		// 이걸 그냥 ge를 생성해서 따로 적용하자
		// InitAttributeDefaults_ServerOnly();
		OnLevelup(StartLevel);
		
		RS_LOG_WARNING(TEXT("Hp : %f, MaxHp : %f, Level : %f, Exp : %f")
				,HealthAttributes->GetHealth(),HealthAttributes->GetMaxHealth(),ExpAttributes->GetExpLevel(),ExpAttributes->GetExpGained())
		
		// 0204  테스트용 
		// ForceNetUpdate();
	}
	
	
	
	// TODO : 타이머 없이 테스트 
	// 타이머 없으면 안된다. 나중에 Restart나 다른곳으로 옮기기 
	FTimerHandle TimerHandle_InitUI;
	GetWorldTimerManager().SetTimer(
		TimerHandle_InitUI, 
		this, 
		&ABase_Character::InitHUDAndUI, 
		0.1f, // 0.01f도 충분할 수 있지만 안전하게 0.1f 추천
		false
	);
	
	// InitHUDAndUI();
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

void ABase_Character::OnExpChange(float NewExp)
{
	AGameplay_PlayerController* PC = Cast<AGameplay_PlayerController>(GetController());
	
	float maxXp = ExpAttributes->GetMaxExpGained();
	
	PC->UpdateExpBar(NewExp/maxXp);
}



void ABase_Character::Cheat_ForceExp()
{
	Server_ForceExp(); // 서버에게 명령
}

void ABase_Character::Server_ForceExp_Implementation()
{
	if (ExpAttributes)
	{
		// GAS 로직 무시하고 강제로 값 변경!
		// 이러면 예측(Prediction) 없이 순수하게 서버 -> 클라 복제만 일어남
		float OldVal = ExpAttributes->GetExpGained();
		ExpAttributes->SetExpGained(OldVal + 50.0f);
        
		UE_LOG(LogTemp, Warning, TEXT("[SERVER CHEAT] Changed Exp to %f"), OldVal + 50.0f);
	}
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
		
		// 지금은 하드코딩으로 넣어주지만 
		// 스킬마다 넣어주는 것은 어려우니
		// 데이터 애셋을 만들어서 클릭으로 동작하는지 홀드로 동작하는지, InputID는 무엇인지 담아 데이터를 가져오도록 하자.
		if (Skill1Action)
		{
			EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill1,true);
			
			EnhancedInputComponent->BindAction(Skill1Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill1,false);
		}
		
		if (Skill2Action)
		{
			EnhancedInputComponent->BindAction(Skill2Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill2,true);
			
			EnhancedInputComponent->BindAction(Skill2Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill2,false);
		}
		
		if (Skill3Action)
		{
			EnhancedInputComponent->BindAction(Skill3Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill3,true);
			
			EnhancedInputComponent->BindAction(Skill3Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill3,false);
		}
		
		if (Skill4Action)
		{
			EnhancedInputComponent->BindAction(Skill4Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill4,true);
			
			EnhancedInputComponent->BindAction(Skill4Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill4,false);
		}
		
		if (Skill5Action)
		{
			EnhancedInputComponent->BindAction(Skill5Action,ETriggerEvent::Started,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill5,true);
			
			EnhancedInputComponent->BindAction(Skill5Action,ETriggerEvent::Completed,this,&ABase_Character::SendAbilityLocalInput,EAbilityInputID::Skill5,false);
		}
	}
}

void ABase_Character::UpdateCharacterClass_Implementation(FAvailableCharacter AvailableCharacter)
{
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

void ABase_Character::CreateHealthWidget(APlayerController* PlayerController)
{
	UUW_HealthBar* HealthBar = Cast<UUW_HealthBar>(CreateWidget<UUW_HealthBar>(PlayerController,HealthBarClass));

	if (!HealthBar)
		return;
	
	HealthBarWidgetReference = HealthBar;
	
	HealthWidget->SetWidget(HealthBarWidgetReference);

	HealthBarWidgetReference->CurrentHp = HealthAttributes->GetHealth();
	HealthBarWidgetReference->MaxHp = HealthAttributes->GetMaxHealth();
	HealthBarWidgetReference->RefreshHpBar();
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



float ABase_Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                  class AController* EventInstigator, AActor* DamageCauser)
{
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
				
		}
	}
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

	// 0204 ASC에서 관리하는 변수들인데 replication을 캐릭터에서 하게되면 클라가 꼬이거나 복제되어 서로 다른 포인터를 가리키게 된다고 한다. by gpt
	// DOREPLIFETIME(ABase_Character, ExpAttributes);
	// DOREPLIFETIME(ABase_Character, HealthAttributes);
	// DOREPLIFETIME(ABase_Character, CombatAttributes);
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




