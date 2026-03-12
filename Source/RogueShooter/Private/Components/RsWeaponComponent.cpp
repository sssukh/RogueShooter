// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RsWeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Utility/RSLog.h"


// Sets default values for this component's properties
URsWeaponComponent::URsWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

FName URsWeaponComponent::GetMuzzleNameByIndex(int32 Index)
{
	if (Index>=MuzzleSocketNames.Num())
		return FName(TEXT("None"));
	
	return MuzzleSocketNames[Index];
}


void URsWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	//
	// // 애셋 체크
	// if (!MuzzleFlashSystem)
	// 	return;
	//
	// // 캐릭터의 메시 컴포넌트 찾기 
	// ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	// if (!OwnerCharacter || !OwnerCharacter->GetMesh()) return;
	//
	//
	// // 적어둔 소켓의 갯수만큼 나이아가라 컴포넌트를 동적 생성 
	// for (const FMuzzleFlashSocketData& SocketData : MuzzleSocketDatas)
	// {
	// 	// // 언리얼에서 컴포넌트를 런타임에 생성하는 정석 코드
	// 	// UNiagaraComponent* NewMuzzleComp = NewObject<UNiagaraComponent>(OwnerCharacter);
	// 	// NewMuzzleComp->SetAsset(MuzzleFlashSystem);
	// 	// NewMuzzleComp->bAutoActivate = true; // 시작하자마자 터지지 않게 설정
	// 	//
	// 	// // 캐릭터의 메시에, 지정된 소켓 위치로 부착합니다.
	// 	// // NewMuzzleComp->SetupAttachment(OwnerCharacter->GetMesh(), MName);
	// 	// NewMuzzleComp->AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,MName);
 //  //       
	// 	// // 월드에 등록하여 실제로 작동하게 만듭니다.
	// 	// NewMuzzleComp->RegisterComponent();
	// 	
	// 	FMuzzleFlash NewMuzzleFlash;
	// 	
	// 	MuzzleFlashComponents.Add(NewMuzzleFlash); 
	// 	
	// }
}


UNiagaraComponent* URsWeaponComponent::GetMuzzleFlashByIndex(int32 MuzzleIndex, bool& MuzzleTrigger)
{
	// // 🌟 안전 검사: 요청한 인덱스가 배열 크기 안에 있는지 반드시 확인! (크래시 방지)
	// if (MuzzleFlashComponents.Num()>MuzzleIndex)
	// {
	// 	if (UNiagaraComponent* TargetFlash = MuzzleFlashComponents[MuzzleIndex].MuzzleFlashComponent)
	// 	{
	// 		// 발사하면서 trigger를 갱신 
	// 		// 이후에 블루프린트에서 trigger 값이나 direction을 설정 
	// 		bool CurrentMuzzleTrigger = MuzzleFlashComponents[MuzzleIndex].MuzzleFlashTrigger;
	// 		MuzzleFlashComponents[MuzzleIndex].MuzzleFlashTrigger = !CurrentMuzzleTrigger;
	// 		MuzzleTrigger = CurrentMuzzleTrigger;
	// 		return TargetFlash;
	// 	}
	// 	else
	// 	{
	// 		// // 해당 인덱스에 존재하지 않으면 새로 생성 
	// 		USkeletalMeshComponent* CharMesh = nullptr;
	// 		if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	// 		{
	// 			CharMesh = Char->GetMesh();
	// 		}
	// 		bool CurrentMuzzleTrigger = MuzzleFlashComponents[MuzzleIndex].MuzzleFlashTrigger;
	// 		MuzzleFlashComponents[MuzzleIndex].MuzzleFlashTrigger = true;
	// 		MuzzleTrigger = true;
	// 		
	// 		return MuzzleFlashComponents[MuzzleIndex].MuzzleFlashComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFlashSystem,CharMesh,MuzzleSocketNames[MuzzleIndex]
	// 			,FVector(),FRotator(0.0f,0.0f,90.0f),EAttachLocation::Type::KeepRelativeOffset,true,true);
	// 		
	// 		
	// 	}
	// }
	return nullptr;
}







