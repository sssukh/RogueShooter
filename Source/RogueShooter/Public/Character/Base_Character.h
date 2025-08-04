// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Interface_CharacterManager.h"
#include "RogueShooter/FlowControlLIbrary.h"
#include "RogueShooter/RSEnumStruct.h"
#include "Base_Character.generated.h"

class AGameplay_PlayerController;
class UWidgetComponent;
class UUW_HealthBar;
class UAbilitiesComponent;
class USG_Player;
struct FAvailableCharacter;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
class IInterface_CharacterManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoad);

UCLASS()
class ROGUESHOOTER_API ABase_Character : public ACharacter, public IInterface_CharacterManager
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void UpdateCharacterClass_Implementation(FAvailableCharacter Character) override;

	UFUNCTION(Server,Reliable)
	void S_SetCharacterMesh(USkeletalMesh* SK);

	UFUNCTION(Server,Unreliable)
	void S_SetCharacterData(FAvailableCharacter CharacterData);

	//*****************************************
	// Character Setup
	//*****************************************
	
	void CreateHealthWidget();
	
	/**
	 * set ref to PC \n
	 */
	void SetupReference();

	
	/**
	 * attempt to load the last used character\n
	 * 마지막으로 사용한 캐릭터를 로드한다.
	 */
	void LoadLastCharacterClass();
	
	/**
	* if you need to setup dispatchers do so here.\n
	* dispatcher를 셋업해야한다면 여기서 하면 된다.
	*/
	void SetupDispatchers();
	 
	//*****************************************
	// Health/Damage
	//*****************************************
	
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Client,Unreliable)
	void Death();

	UFUNCTION(NetMulticast,Unreliable)
	void MC_Death();

	// Interface 상속 
	UFUNCTION()
	virtual void RestoreHealth_Implementation(float amount) override;

	// server 
	UFUNCTION(Server,Unreliable)
	void S_RestoreHealth(float amount);

	//*****************************************
	// Widgets
	//*****************************************

	UFUNCTION(Client,Unreliable)
	void OC_SetupWidgets();
	
	UFUNCTION(NetMulticast,Unreliable)
	void MC_UpdateHealthBar(float percent);

	virtual void SetupHealthWidget_Implementation() override;

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

	UFUNCTION()
	virtual void Pause_Implementation(bool Pause, bool Override) override;

	//*****************************************
	// Passive Stats
	//*****************************************
	
	/**
	* Adjust stats on pawn via interface to avoid circular dependencies on ability component
	* ability component의 의존성을 피하기 위해 interface를 통해 pawn의 스탯을 조정한다.
	*/
	
	/**
	* Call Server to Update character specific stats
	* 서버를 호출해서 캐릭터 스탯을 업데이트한다.
	*/
	UFUNCTION()
	virtual void AdjustPassive_Implementation(EPassiveAbilities Stat, float MultiplicationAmount) override;

	UFUNCTION(Server,Reliable)
	void S_UpdatePassiveStat(EPassiveAbilities Stat,float Value);

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
	
	// Components
private:
	UPROPERTY()
	TObjectPtr<USphereComponent> AbilitySphere;

	UPROPERTY()
	TObjectPtr<UAbilitiesComponent> AbilityComponent;

	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY()
	TObjectPtr<UWidgetComponent> HealthWidget;

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

	UPROPERTY(ReplicatedUsing="OnRep_CharSK",VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	TObjectPtr<USkeletalMesh> CharSK;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	EActiveAbilities StartingAbility = EActiveAbilities::Hammer;

	// Character
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	int32 NeededXP = 0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	float CurrentHealth = 100.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	float MaxHealth = 100.0f;
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	TObjectPtr<UUW_HealthBar> HealthBarWidgetReference;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	int32 CurrentXP =0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Character Setup")
	bool IsDead = false;

	
	//
	FDoOnce DeathDoOnce;

	UPROPERTY()
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	UPROPERTY()
	TObjectPtr<UAnimInstance> CharacterAnimInstance;

	TSubclassOf<UUW_HealthBar> HealthBarClass;

};
