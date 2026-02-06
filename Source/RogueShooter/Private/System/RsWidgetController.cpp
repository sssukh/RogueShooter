// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Data/ExpSet.h"
#include "GameplayAbility/GA_Skill.h"
#include "RogueShooter/AssetPath.h"
#include "GameplayEffectTypes.h"
#include "Utility/FRsGameplayTags.h"
#include "Utility/RSLog.h"

URsWidgetController::URsWidgetController()
{
	const ConstructorHelpers::FObjectFinder<UDataTable> SkillInfoTableFinder(*AssetPath::DataTable::DT_SkillInfo);
	
	if (SkillInfoTableFinder.Succeeded())
		SkillInfoDataTable = SkillInfoTableFinder.Object;
}

void URsWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WcParams)
{
	AbilitySystemComponent = WcParams.AbilitySystemComponent;
	PlayerState = WcParams.PlayerState;
	PlayerController = WcParams.PlayerController;
	
	if (AbilitySystemComponent)
	{
		CharExpSet = Cast<UExpSet>(AbilitySystemComponent->GetAttributeSet(UExpSet::StaticClass()));
	}
	
	// BindCallbacksToDependencies();
}

void URsWidgetController::BindCallbacksToDependencies()
{
	if (!IsValid(AbilitySystemComponent))
	{
		RS_LOG_ERROR(TEXT("ASC is NULL"))
		return;
	}
	
	if (!CharExpSet)
	{
		RS_LOG_ERROR(TEXT("ExpSet is NULL"))
		return;
	}
	
	// 디버깅용 로그: 속성이 유효한지 확인
	FGameplayAttribute ExpAttribute = CharExpSet->GetExpGainedAttribute();
	if (!ExpAttribute.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL: ExpGainedAttribute is Invalid! Check Macros in AttributeSet.h"));
		return;
	}

	// ASC가 해당 속성을 가지고 있는지 확인 (값이 있는지)
	if (!AbilitySystemComponent->HasAttributeSetForAttribute(ExpAttribute))
	{
		UE_LOG(LogTemp, Error, TEXT("CRITICAL: ASC does not have CharExpSet registered!"));
		return;
	}
	
	if (CharExpSet)
	{
		// Exp가 변하면 방송
		FDelegateHandle Handle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharExpSet->GetExpGainedAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnExpChanged.Broadcast(Data.NewValue);
				}
		);
		
		// Max Exp가 변하면 방송 
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharExpSet->GetMaxExpGainedAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnMaxExpChanged.Broadcast(Data.NewValue);
				}
		);
		
		// Level이 변하면 방송 
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			CharExpSet->GetExpLevelAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnLevelChanged.Broadcast(Data.NewValue);
				}
		);
	}
	
	// [핵심] 우리가 만든 DataAsset을 순회하거나, 
	// 혹은 미리 알고 있는 '모든 쿨타임 태그'를 등록합니다.
	

	for (const FGameplayTag& Tag : TagsToListen)
	{
		// ⭐ 태그 리스너 등록 ⭐
		// AddUObject는 추가적인 인자(Tag)를 넘길 수 없으므로, 람다(Lambda)를 쓰거나 
		// 바인딩 시 파라미터를 캡처하는 방식을 써야 하지만, 가장 쉬운 방식은 아래와 같습니다.
        
		AbilitySystemComponent->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		   .AddLambda([this, Tag](const FGameplayTag CallbackTag, int32 NewCount)
		   {
			   // 람다 함수 안에서 처리
			   OnCooldownTagChanged(Tag, NewCount);
		   });
	}
}

void URsWidgetController::BroadcastInitialValues()
{
	// 1. ExpSet이 있는지 확인
	if (CharExpSet)
	{
		// 2. 현재 값(Current Value)을 읽어서 방송
		OnExpChanged.Broadcast(CharExpSet->GetExpGained());
		OnMaxExpChanged.Broadcast(CharExpSet->GetMaxExpGained());
		OnLevelChanged.Broadcast(CharExpSet->GetExpLevel());
        
		// (로그 확인용)
		RS_LOG_SCREEN(TEXT("[WidgetController] Broadcasting Initial Values: Exp=%f, Max=%f"), 
			CharExpSet->GetExpGained(), CharExpSet->GetMaxExpGained());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[WidgetController] Cannot Broadcast Initial Values: CharExpSet is NULL"));
	}
}

void URsWidgetController::SetTagsToListen(FGameplayTagContainer Tags)
{
	for (int i = 0; i < Tags.Num(); ++i)
	{
		TagsToListen.Add(Tags.GetByIndex(i));
	}
}

bool URsWidgetController::GetCooldownInfo(const FGameplayTag& CooldownTag, float& OutRemaining, float& OutDuration)
{
	if (!AbilitySystemComponent) return false;

	// 1. 쿼리 생성: 해당 태그를 포함하는 이펙트를 찾겠다
	FGameplayEffectQuery Query;
	Query.CustomMatchDelegate.BindLambda([&](const FActiveGameplayEffect& ActiveEffect)
	{
		// 이펙트가 부여한 태그 중에 우리 태그가 있는지 확인
		FGameplayTagContainer GrantedTags;
		ActiveEffect.Spec.GetAllGrantedTags(GrantedTags);
		return GrantedTags.HasTag(CooldownTag);
	});

	// 2. 쿼리 실행: 조건에 맞는 모든 이펙트 가져오기
	TArray<float> Durations;
	TArray<float> Remainings;
    
	// ASC가 제공하는 함수로 시간 가져오기 (가장 긴 시간 기준)
	// 결과: 조건에 맞는 이펙트 중 '가장 긴' 종료 시간과 지속 시간을 반환함
	TArray<FActiveGameplayEffectHandle> ActiveEffects = AbilitySystemComponent->GetActiveEffects(Query);
    
	float LongestRemaining = -1.0f;
	float LongestDuration = 0.0f;

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffects)
	{
		const FActiveGameplayEffect* Effect = AbilitySystemComponent->GetActiveGameplayEffect(Handle);
		if (Effect)
		{
			float Remaining = Effect->GetTimeRemaining(GetWorld()->GetTimeSeconds());
			float Duration = Effect->GetDuration();

			// 여러 개의 쿨타임이 겹쳤다면 가장 긴 것을 선택
			if (Remaining > LongestRemaining)
			{
				LongestRemaining = Remaining;
				LongestDuration = Duration;
			}
		}
	}

	if (LongestRemaining > 0.0f)
	{
		OutRemaining = LongestRemaining;
		OutDuration = LongestDuration;
		return true;
	}

	return false;
}

void URsWidgetController::BroadcastInitialAbilityInfo()
{
	if (!AbilitySystemComponent || !SkillInfoDataTable) return;

	// 1. ASC가 현재 가지고 있는(Activatable) 모든 어빌리티를 가져옴
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		// 2. 스킬 인스턴스(또는 CDO)를 가져와서 우리가 만든 Base 클래스로 캐스팅
		// (CDO: Class Default Object, 생성되지 않은 원본 데이터)
		UGA_Skill* Ability = Cast<UGA_Skill>(Spec.Ability);
        
		if (Ability)
		{
			// 3. GA_BaseSkill에 설정해둔 AbilityTag 가져오기
			// FGameplayTag Tag = Ability->AbilityTag; // (public이나 Getter 필요)

			// 4. 태그 이름을 Key로 데이터 테이블 검색
			// "Ability.Attack.FireBall"이라는 이름의 Row를 찾음
			FSkillInfo* Row = SkillInfoDataTable->FindRow<FSkillInfo>(Ability->AbilityTag.GetTagName(), TEXT(""));
			
			// 알맞은 스킬슬롯을 찾기위한 스킬슬롯 태그 
			FGameplayTag SkillSlotTag = Ability->CooldownTags.GetByIndex(0);
			
			if (Row)
			{
				if (OnSkillInfoLoaded.IsBound())
					// 5. 찾았으면 방송! "파이어볼 태그에 대한 정보는 이거야!"
					OnSkillInfoLoaded.Broadcast(SkillSlotTag, *Row);
			}
		}
	}
}

void URsWidgetController::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	bool bIsCooldown = NewCount > 0;
	float Duration = 0.0f;

	float TimeRemaining = 0.0f;
	// 쿨타임 시작이면 남은 시간을 구해서 같이 보내줍니다.
	if (bIsCooldown && AbilitySystemComponent)
	{
		// (참고) GetCooldownTimeRemaining 같은 함수는 ASC에 없으므로
		// QueryGameplayEffectSpec 등을 통해 가져와야 하지만,
		// 여기서는 일단 태그 상태(ON/OFF)를 보내는 것에 집중합니다.
		// 필요하다면 가장 긴 Duration을 가진 GE를 찾는 로직 추가 필요.
		GetCooldownInfo(CooldownTag,TimeRemaining,Duration);
	}

	// 📢 방송 송출! (나를 구독하고 있는 모든 위젯에게 알림)
	OnCooldownChange.Broadcast(CooldownTag, bIsCooldown, Duration);
}
