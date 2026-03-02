// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_ReticleWidgetBase.h"

#include "Character/Base_Character.h"

void UUW_ReticleWidgetBase::InitializeReticle(AActor* InWeaponOrCharacter)
{
	TargetActor = InWeaponOrCharacter;
}

void UUW_ReticleWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TargetActor)
	{
		// 1. TargetActor에서 현재 탄퍼짐 값(CurrentSpread)을 가져옵니다.
		// (무기 클래스에 GetCurrentSpread() 같은 함수가 있다고 가정)
		
		float Spread = 0;
		
		if (ABase_Character* Character = Cast<ABase_Character>(TargetActor))
		{
			Spread = Character->CurrentSpread;
		}

		// 2. 블루프린트 이벤트 호출!
		OnSpreadUpdated(Spread);
	}
}
