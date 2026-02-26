// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_SendGameplayEvent.generated.h"

class URsWeaponFireConfig;
/**
 * 
 */
UCLASS()
class ROGUESHOOTER_API URsAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
public:
	// 1. 보낼 태그 (예: Event.Montage.Fire.Left)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilities")
	FGameplayTag EventTag;

	// 2. 전달할 수치 (필요하면 사용, 없으면 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayAbilities")
	float EventMagnitude = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TObjectPtr<URsWeaponFireConfig> FireConfig;
	
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
