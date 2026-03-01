// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Interface_CharacterManager.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "RogueShooter/RSEnumStruct.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "RsBaseCharacter.h"
#include "Base_Character.generated.h"

class UGA_Skill;
class UUnitWidgetController;
class UExpSet;
class AFloatingTextActor;
class UHealthSet;
class UCombatSet;
class UInventoryComponent;
class AGameplay_PlayerController;
class UWidgetComponent;
class UUW_HUDHealthBar;
class UAbilitiesComponent;
class USG_Player;
struct FAvailableCharacter;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class IInterface_CharacterManager;
class UAbilitySystemComponent;
class UCharAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoad);

// USTRUCT(BlueprintType)
// struct FGAbilityID
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(EditAnywhere,BlueprintReadOnly)
// 	TSubclassOf<UGameplayAbility> GameplayAbility;
// 	
// 	UPROPERTY(EditAnywhere,BlueprintReadOnly)
// 	EAbilityInputID InputID;
// };

USTRUCT(BlueprintType)
struct FSkillInputMapping
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<UGA_Skill> Ability;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	EAbilityInputID InputID;
};

UCLASS()
class ROGUESHOOTER_API ABase_Character : public ARsBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;	
	
	virtual void OnRep_Controller() override;
	
	virtual void OnRep_PlayerState() override;
/**
 *	GAS
 */
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void AddCharacterAbilities();
	
	
protected:
	// 1. GAS 엔진
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	// TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 부여할 어빌리티 목록(자동으로 발동)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category =  "GAS | Config")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	// 부여할 스킬 목록(버튼을 눌러 발동)
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category =  "GAS | Config")
	TArray<FSkillInputMapping> DefaultSkills;
	
	// 초기화 함수 (아래 설명 참조)
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	void OnLevelup(float NewLevel);
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "GAS|Config")
	TSubclassOf<UGameplayEffect> DefaultCurveEffectClass;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Config | Attribute")
	FCurveTableRowHandle MaxXpCurve;
	

	
public:
	float GetMaxXpForLevel(float pLevel) const;
	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	


	//*****************************************
	// Interface_CharacterManager
	//*****************************************

	
	
	UFUNCTION()
	virtual void Pause_Implementation(bool Pause, bool Override) override;

	/**
	* Call Server to Update character specific stats
	* 서버를 호출해서 캐릭터 스탯을 업데이트한다.
	*/

	virtual bool IsAlive_Implementation() override;
	
	virtual ABase_Character* GetCharacter_Implementation() override;
	

	//*****************************************
	// Widget 
	//*****************************************
	
	
	//*****************************************
	// Character Setup
	//*****************************************
	
	/**
	 * set ref to PC \n
	 */
	void SetupReference();

	
	/**
	 * attempt to load the last used character\n
	 * 마지막으로 사용한 캐릭터를 로드한다.
	 */
	void LoadLastCharacterClass();
	
	
	//*****************************************
	// Health/Damage
	//*****************************************

	UFUNCTION(Client,Unreliable)
	void Death();

	UFUNCTION(NetMulticast,Unreliable)
	void MC_Death();

	
	virtual void Die(AActor* DamageCauser) override;

	


	//*****************************************
	// Pause Logic
	//*****************************************
	// Unreal은 Blueprint에서 realtime Pause를 지원하지않아서 time dilation 값을 매우 낮게 설정한다.
	// TODO : C++는 다른 방법이 있을테니 이를 수정해서 적용하자.
	UFUNCTION(Server,Reliable)
	void S_Pause(bool Pause, bool Override);

	UFUNCTION(NetMulticast,Reliable)
	void MC_Pause(bool Pause);

	UFUNCTION(Client,Reliable)
	void OC_Pause(bool Pause, bool Override);

	

	//*****************************************
	// Passive Stats
	//*****************************************
	
	/**
	* Adjust stats on pawn via interface to avoid circular dependencies on ability component
	* ability component의 의존성을 피하기 위해 interface를 통해 pawn의 스탯을 조정한다.
	*/
	
	


	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Function is so we can override on children class since you cannot override on rep notifies\n
	 * 
	 */
	void OROnRepCharacterClass();
	
	UFUNCTION()
	void OnRep_Character();

	/**
	 * set skeletal mesh for multiplayer 
	 */
	UFUNCTION()
	void OnRep_CharSK();
	
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;

	// Input
protected:
	/**
	 * 
	 */
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	void Move(const FInputActionValue& Value);
	
	void CharacterInputSetting();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> Skill1Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> Skill2Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> Skill3Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> Skill4Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> Skill5Action;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Config | Input", meta = (AllowPrivateAccess)) 
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	void SendAbilityLocalInput(const EAbilityInputID InputID,bool bIsPressed);
	// Character Setup
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	TObjectPtr<AGameplay_PlayerController> LocalPlayerController;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Setup")
	TObjectPtr<UObject> GM_Interface;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	TObjectPtr<USG_Player> GameSave;

	UPROPERTY(ReplicatedUsing="OnRep_Character",VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	FAvailableCharacter Character;



	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	bool IsDead = false;

	//
	FDoOnce DeathDoOnce;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Config")
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Config")
	// TObjectPtr<UAnimInstance> CharacterAnimInstance;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Config")
	TSubclassOf<AFloatingTextActor> FloatingActorClass;
	
	UPROPERTY()
	int32 StartLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | UI")
	TSubclassOf<UUnitWidgetController> CharacterWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UUnitWidgetController> CharacterWidgetController;
	
	bool bIsGASInitialized = false;
};
