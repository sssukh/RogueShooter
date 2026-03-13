// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Skill.generated.h"


UENUM(BlueprintType)
enum class ESkillInputStyle : uint8
{
	// 단발
	Instant     UMETA(DisplayName = "Instant (One Shot)"), 
	// 연사
	Continuous  UMETA(DisplayName = "Continuous (Hold)"),  
	// 차징 (모아서 쏘기)
	Charging    UMETA(DisplayName = "Charging (Release)"), 
	// 점사 (3점사 등)/직접 EndAbility 호출해야함.
	Burst       UMETA(DisplayName = "Burst Fire")          
};


/**
 * BaseSkill 클래스 
 * 
 * 모든 스킬에서 사용할 쿨타임 시스템을 적용함
 * 
 * 새로운 스킬을 추가할 때 마다 RSGameplayTag 클래스 내부에 스킬 태그를 추가할 것.
 */
UCLASS()
class ROGUESHOOTER_API UGA_Skill : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Skill();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|Interface", DisplayName = "On Activate Skill")
	void BP_OnActivateSkill();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	/**
	 * 블루프린트 에디터에서 설정한 Cooldown GE Class를 CommitAbility로 간단히 실행 가능하도록 오버라이드
	 * SetByCaller를 통해 동적으로 쿨타임을 제어한다.
	 * 
	 * @param Handle 
	 * @param ActorInfo 
	 * @param ActivationInfo 
	 */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	// 쿨다운 여부를 
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	
	FORCEINLINE ESkillInputStyle GetSkillInputStyle() const {return InputStyle;}
	// Utility Helpers
protected:
	UFUNCTION(BlueprintCallable, Category = "GAS|Helper")
	FVector GetMouseCursorLocation();

	// ChargeAmount: 0.0 ~ 1.0 사이의 값 (차징일 때만 변하고, 나머지는 항상 1.0)
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Logic")
	void ExecuteSkillLogic(float ChargeAmount);
	virtual void ExecuteSkillLogic_Implementation(float ChargeAmount);
	
	
protected:
	// 스킬 타입 (단발 vs 연사)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config")
	ESkillInputStyle InputStyle = ESkillInputStyle::Instant;

	
	// false일 시 직접 EndAbility 반드시 호출 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Instant"))
	bool bAutoEndAbility = true;
	
	// 연사 속도 (초 단위, Continuous 모드일 때만 사용)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Continuous"))
	float FireRate = 0.1f;
	
	// 차징 관련 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Charging"))
	float MinChargeTime = 0.0f; // 최소 이만큼은 눌러야 발사됨

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Charging"))
	float MaxChargeTime = 2.0f; // 최대 충전 시간 (이 이상 눌러도 게이지 100%)

	// 점사 관련 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Burst"))
	int32 MaxBurstCount = 3; // 한 번 누르면 몇 발 나가는지
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Burst"))
	int32 CurrentBurstCount = 0; // 지금 몇 발 나갔는지
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Config", meta = (EditCondition = "InputStyle == ESkillInputStyle::Burst"))
	float BurstTime = 3.0f; // 몇초동안 나가는지 
	
protected:
	// 버튼을 뗐을 때 감지 (Task 콜백)
	UFUNCTION()
	void OnReleaseInput(float TimeHeld);

	// 타이머 핸들 (반복 실행용)
	FTimerHandle TimerHandle_Loop;

	// 반복 실행될 함수
	void LoopLogic();
	
	
	float ChargeStartTime = 0.0f; // 차징 시작 시간

	// 점사 로직용 함수
	void BurstLogic();
	
	// 태스크의 발사 신호를 받을 C++ 콜백 함수
	UFUNCTION()
	void HandlePerformAction(int32 ActionNumber);

	// 태스크의 완료 신호를 받을 C++ 콜백 함수
	UFUNCTION()
	void HandleRepeatFinished(int32 ActionNumber);
	
public:
	// 데이터 애셋에 접근할 키 값 
	// 스킬 아이콘 및 설명을 가져옴 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	FGameplayTag AbilityTag;
	
	// 쿨타임 설정 값
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup")
	float CooldownDuration;
	
	// 쿨타임에 관여할 태그들을 담은 태그 컨테이너 
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTagContainer CooldownTags;
	
	// 외부에서 쿨타임을 설정할 때 필요한 태그
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "GAS | Config | Setup") 
	FGameplayTag CooldownDurationTag;
	
	// 실행할 애니메이션 (없으면 안 함)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Visual")
	TObjectPtr<UAnimMontage> SkillMontage;
    
	// 몽타주 재생 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Visual")
	float MontageRate = 1.0f;
	
	// [Policy] 스킬 시작 시 자동으로 비용과 쿨타임을 처리할지 여부
	// - True (기본값): 파이어볼, 힐 등 즉발 스킬용. (기존 방식)
	// - False: 차징, 연사, 홀드 스킬용. (BP에서 수동 처리)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Policy")
	bool bAutoCommit = true;
};
