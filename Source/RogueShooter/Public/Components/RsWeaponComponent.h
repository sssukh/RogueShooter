// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraComponent.h"
#include "RsWeaponComponent.generated.h"

// USTRUCT(BlueprintType)
// struct FMuzzleFlashSocketData
// {
// 	GENERATED_BODY()
// 	
// 	UPROPERTY(BlueprintReadWrite)
// 	FName MuzzleFlashSocket;
// 	
// 	UPROPERTY(BlueprintReadWrite)
// 	bool MuzzleFlashTrigger = true;
// };

/*
 *	무기의 이펙트 관련 데이터 및 실행 함수를 가진 컴포넌트
 *	Lyra를 참고했는데 Lyra의 경우 무기가 동적으로 바뀔 수 있지만
 *	이 게임은 캐릭터가 정해지면 무기가 변하지 않으므로 NiagaraComponent를 부착해서 재사용하도록 함.
 *	
 *	lyra의 이펙트는 NS의 User.Trigger가 토글되어 입력되어 이전의 입력된 값과 달라야 trigger되도록 되어있다.
 *	
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUESHOOTER_API URsWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	URsWeaponComponent();

	UFUNCTION(BlueprintCallable)
	FName GetMuzzleNameByIndex(int32 Index);
protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	UNiagaraComponent* GetMuzzleFlashByIndex(int32 MuzzleIndex, bool& MuzzleTrigger);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon|FX|Muzzle")
	TObjectPtr<UNiagaraSystem> MuzzleFlashSystem;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon|FX|Tracer")
	TObjectPtr<UNiagaraComponent> TracerComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon|FX|Tracer")
	TObjectPtr<UNiagaraSystem> TracerSystem;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Weapon|FX|Tracer")
	bool TracerTrigger;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Weapon | Config") 
	TArray<FName> MuzzleSocketNames;
};
