// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/RsAnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Data/RsWeaponFireConfig.h"

FString URsAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	// 몽타주 트랙에 태그 이름으로 표시되게 함 (편의성)
	if (EventTag.IsValid())
	{
		return EventTag.ToString();
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		// Payload 생성
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = EventMagnitude;
		Payload.Instigator = MeshComp->GetOwner();
		Payload.Target = MeshComp->GetOwner();
		Payload.OptionalObject = FireConfig;

		// 이벤트 전송
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Payload);
	}
}
