// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RsWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Utility/FRsGameplayTags.h"

URsWidgetController::URsWidgetController()
{
}

void URsWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WcParams, FGameplayTagContainer InTagsToListen)
{
	ASC = WcParams.AbilitySystemComponent;
	PS = WcParams.PlayerState;
	PC = WcParams.PlayerController;
	
	SetTagsToListen(InTagsToListen);
	
	BindCallbacksToDependencies();
}

void URsWidgetController::BindCallbacksToDependencies()
{
	if (!ASC) return;

	// [핵심] 우리가 만든 DataAsset을 순회하거나, 
	// 혹은 미리 알고 있는 '모든 쿨타임 태그'를 등록합니다.
	// (예시를 위해 단일 태그 등록이 아니라, '태그 목록'을 순회한다고 가정합니다.)
    
	// 예: FireBall, IceBolt 등의 태그를 배열로 가지고 있다고 가정
	// TArray<FGameplayTag> TagsToListen; 
	// TagsToListen.Add(FRsGameplayTags::Get().Status_Cooldown_SKill1);
	// TagsToListen.Add(FRsGameplayTags::Get().Status_Cooldown_SKill2);
	// TagsToListen.Add(FRsGameplayTags::Get().Status_Cooldown_SKill3);
	// TagsToListen.Add(FRsGameplayTags::Get().Status_Cooldown_SKill4);
	// TagsToListen.Add(FRsGameplayTags::Get().Status_Cooldown_SKill5);
	

	for (const FGameplayTag& Tag : TagsToListen)
	{
		// ⭐ 태그 리스너 등록 ⭐
		// AddUObject는 추가적인 인자(Tag)를 넘길 수 없으므로, 람다(Lambda)를 쓰거나 
		// 바인딩 시 파라미터를 캡처하는 방식을 써야 하지만, 가장 쉬운 방식은 아래와 같습니다.
        
		ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
		   .AddLambda([this, Tag](const FGameplayTag CallbackTag, int32 NewCount)
		   {
			   // 람다 함수 안에서 처리
			   OnCooldownTagChanged(Tag, NewCount);
		   });
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
	if (!ASC) return false;

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
	TArray<FActiveGameplayEffectHandle> ActiveEffects = ASC->GetActiveEffects(Query);
    
	float LongestRemaining = -1.0f;
	float LongestDuration = 0.0f;

	for (const FActiveGameplayEffectHandle& Handle : ActiveEffects)
	{
		const FActiveGameplayEffect* Effect = ASC->GetActiveGameplayEffect(Handle);
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

void URsWidgetController::OnCooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount)
{
	bool bIsCooldown = NewCount > 0;
	float Duration = 0.0f;

	float TimeRemaining = 0.0f;
	// 쿨타임 시작이면 남은 시간을 구해서 같이 보내줍니다.
	if (bIsCooldown && ASC)
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
