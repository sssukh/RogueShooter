// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/GA_GunFire.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Base_Projectile.h"
#include "Character/Base_Character.h"
#include "Data/Attribute/CrosshairAttSet.h"
#include "GameFramework/Character.h"
#include "Utility/FRsGameplayTags.h"


UGA_GunFire::UGA_GunFire()
{
}

void UGA_GunFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	// 🎧 [서버] 클라이언트가 쏘아올릴 TargetData를 받을 수 있도록 귀를 열어둡니다.
	if (HasAuthority(&CurrentActivationInfo))
	{
		ASC->AbilityTargetDataSetDelegate(Handle, ActivationInfo.GetActivationPredictionKey()).AddUObject(this, &UGA_GunFire::OnTargetDataReceived);
	}
}

void UGA_GunFire::ApplyDamage(AActor* TargetActor)
{
	
	if (!DamageEffectClass||!TargetActor)
		return;
	
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextContext = SourceASC->MakeEffectContext();
	
	ContextContext.SetAbility(this);
	ContextContext.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextContext);
	
	// 1. 적의 ASC 찾기
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	// 2. 명세서(Spec)가 유효하고 적에게 ASC가 있다면 적용
	if (SpecHandle.IsValid() && TargetASC)
	{
		// 내(Instigator)가 만든 명세서를 적(Target)에게 적용한다.
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
}

void UGA_GunFire::TriggerFireGameplayCue(FVector MuzzleLoc, FVector TargetLoc, const FHitResult& HitResult)
{
	FGameplayCueParameters CueParams;
    
	// 1. 시작점 (총구) -> Location에 담음 (약속하기 나름)
	CueParams.Location = MuzzleLoc;
    
	// 2. 가짜 HitResult 만들기 (허공에 쐈을 때를 대비)
	FHitResult CueHitResult = HitResult;
	if (!CueHitResult.bBlockingHit)
	{
		// ⚠️ 안 맞았을 때는 ImpactPoint가 0,0,0이므로, 끝점(TraceEndLoc)을 강제로 넣어줍니다!
		CueHitResult.ImpactPoint = TargetLoc; 
	}

	// 3. ⭐ [핵심] EffectContext 보따리를 만들고 HitResult 넣기
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.AddHitResult(CueHitResult); // 보따리에 HitResult 담기

	// 4. 파라미터에 보따리 연결
	CueParams.EffectContext = ContextHandle;

	// 5. GC 실행
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(
		FRsGameplayTags::Get().GC_Weapon_Fire, 
		CueParams
	);
}

void UGA_GunFire::ExecuteSkillLogic_Implementation(float ChargeAmount)
{
	// 1. 대미지 GE 클래스 가져오기 (블루프린트에서 설정)
	if (!DamageEffectClass || !ProjectileClass)
	{
		return;
	}

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextContext = SourceASC->MakeEffectContext();
	
	ContextContext.SetAbility(this);
	ContextContext.AddSourceObject(this);
	// ContextHandle.AddInstigator(Avatar,Weapon); // 무기정보 등을 넣기도 한다.
	
	// 2. 명세서(Spec) 생성
	// MakeOutgoingSpec을 하면 내(Source) 스탯을 기반으로 Context가 생성됩니다.
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextContext);

	// 3. (옵션) 동적으로 값 변경하기
	// 예: 차지샷이라서 대미지를 2배로 늘려야 한다면?
	// SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Data.DamageMultiplier")), 2.0f);

	// 4. 투사체 스폰을 위한 Transform 계산
	FTransform SpawnTransform = GetAvatarActorFromActorInfo()->GetActorTransform(); 
	// 실제로는 총구 소켓(Muzzle Socket) 위치를 가져와야 함
	// 임시
	FVector Forward = SpawnTransform.GetRotation().GetForwardVector();
	SpawnTransform.SetLocation(SpawnTransform.GetLocation() + SpawnTransformOffset);
	SpawnTransform.SetRotation(Forward.ToOrientationQuat());
	// 5. 투사체 스폰 (Deferred Spawn 사용 추천)
	// Deferred Spawn을 쓰면 스폰 완료(FinishSpawning) 전에 변수를 세팅할 수 있습니다.
	ABase_Projectile* Projectile = GetWorld()->SpawnActorDeferred<ABase_Projectile>(
		ProjectileClass, 
		SpawnTransform, 
		GetOwningActorFromActorInfo(), 
		Cast<APawn>(GetOwningActorFromActorInfo()), 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Projectile)
	{
		// ⭐ 여기서 명세서를 배달부에게 쥐어줍니다!
		Projectile->DamageEffectSpecHandle = SpecHandle;

		// 스폰 완료
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UGA_GunFire::SendTargetDataToServer(const FHitResult& HitResult)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

	// TargetData 보따리 생성 및 HitResult 담기
	FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit();
	HitData->HitResult = HitResult;

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(HitData);

	// GAS 예측(Prediction) 키 발급 - "이 데이터는 내가 쏜 거야!" 라는 꼬리표
	FScopedPredictionWindow ScopedPrediction(ASC, true);

	// 내가 권한이 없는 클라이언트라면 서버로 데이터 쏘기
	if (!HasAuthority(&CurrentActivationInfo))
	{
		ASC->CallServerSetReplicatedTargetData(
			CurrentSpecHandle,
			CurrentActivationInfo.GetActivationPredictionKey(),
			TargetDataHandle,
			FGameplayTag(),
			ASC->ScopedPredictionKey
		);
	}
	else
	{
		// 내가 이미 호스트(서버 권한 있음)라면 네트워크로 보낼 필요 없이 바로 수신 함수 호출
		OnTargetDataReceived(TargetDataHandle, FGameplayTag());
	}
}

void UGA_GunFire::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

	// 🧹 메모리 누수 방지: 서버 캐시 비우기
	ASC->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

	// 데이터가 정상적으로 들어왔는지 확인
	if (Data.Data.Num() > 0)
	{
		const FHitResult* HitResult = Data.Data[0]->GetHitResult();
		if (HitResult)
		{
			// 🛡️ [검증 로직 (선택)] 거리가 너무 멀거나 벽을 관통했다면 여기서 데미지를 취소할 수 있습니다.

			// 💥 [실제 데미지 적용] 
			if (HitResult->GetActor())
			{
				// ApplyGameplayEffectToTarget(...) 을 호출하여 체력 차감
				ApplyDamage(HitResult->GetActor());
			}

			// 🎇 [다른 클라이언트들에게 이펙트 뿌리기]
			// 이 TriggerFireGameplayCue는 서버가 실행하여 모두에게 전파(Multicast)됩니다.
			// 하지만 GAS의 PredictionKey 시스템 덕분에, 방금 이펙트를 띄운 클라이언트 화면에는 중복해서 그리지 않습니다!
			FVector MuzzleLoc = HitResult->TraceStart; // 필요시 소켓 이름 동기화 로직 추가
			FVector TargetLoc = HitResult->bBlockingHit ? HitResult->ImpactPoint : HitResult->TraceEnd;
            
			TriggerFireGameplayCue(MuzzleLoc, TargetLoc, *HitResult);
		}
	}
}

void UGA_GunFire::FireHitScan(FName SocketName)
{
	// 🌟 1. 오직 '나(직접 조종하는 클라이언트 또는 호스트)'만 발사 연산을 합니다.
	if (!CurrentActorInfo->IsLocallyControlled()) return;
	
	ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	APlayerController* PC = Cast<APlayerController>(Avatar->GetController());
	if (!Avatar || !PC) return;

	FVector CameraLoc;
	FRotator CameraRot;
	
	// 1. ⭐ 카메라의 위치와 회전을 가져옵니다. (화면 정중앙 기준)
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	// 2. 레이캐스트 방향 설정
	FVector CameraFwd = CameraRot.Vector();
	
	// 1. 시작점: 소켓 위치 (Muzzle_L or Muzzle_R)
	FVector MuzzleLoc = Avatar->GetMesh()->GetSocketLocation(SocketName);

	FVector CameraToMuzzle = MuzzleLoc - CameraLoc;
	
	float ProjectedDist = FVector::DotProduct(CameraToMuzzle,CameraFwd);
	
	FVector Start = CameraLoc + (CameraFwd * ProjectedDist);
	
	
    
	// 3. 확산(Spread) 적용 (로그라이크의 핵심!)
	// 현재 Spread 속성(Attribute)을 가져와서 랜덤하게 방향을 틉니다.
	// 임시로 상수를 넣고 로직 완성 이후에 발사 시간에 비례해서 커지는 값을 넣자 
	// float SpreadAngle = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(URsAttributeSet::GetSpreadAttribute());
	float SpreadAngle = 3.0f;
	if (ABase_Character* Character = Cast<ABase_Character>(GetActorInfo().AvatarActor))
	{
		SpreadAngle = Character->CurrentSpread;
	}
	FVector ShootDir = FMath::VRandCone(CameraFwd, FMath::DegreesToRadians(SpreadAngle));

	// 4. 끝점: 사거리(Range) 적용
	float Range = 3000.0f; // Attribute에서 가져오면 더 좋음
	FVector End = Start + (ShootDir * Range);

	// 5. 레이캐스트 실행
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Avatar); // 나는 맞으면 안 됨

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, Start, End, ECC_Visibility, Params
	);

#if ENABLE_DRAW_DEBUG
	// 1. 총알 궤적 선 그리기 (빨간색, 2초 동안 유지, 두께 1.0)
	DrawDebugLine(
		GetWorld(), 
		Start, 
		End, 
		FColor::Red, 
		false, 
		2.0f, 
		0, 
		1.0f
	);

	// 2. 무언가에 맞았다면, 맞은 위치에 점 찍기 (초록색, 2초 유지, 크기 10.0)
	if (bHit)
	{
		DrawDebugPoint(
			GetWorld(), 
			HitResult.ImpactPoint, 
			10.0f, 
			FColor::Green, 
			false, 
			2.0f
		);
        
		// 맞은 액터의 이름을 로그로 출력해보기
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());
	}
#endif
	
	// 6. 결과 처리
	if (bHit)
	{
		// 적을 맞췄다면? -> ApplyGameplayEffect (Damage)
		if (AActor* HitActor = HitResult.GetActor())
		{
			// GAS 대미지 적용 로직...
			ApplyDamage(HitActor);
		}
        
		// 피격 이펙트 (GameplayCue: Impact)
		// HitResult.Location에서 파티클 재생
	}
	// 타겟 지점 계산 (맞았으면 ImpactPoint, 안 맞았으면 허공의 끝점)
	FVector BeamTarget = bHit ? HitResult.ImpactPoint : End;
    
	HitResult.TraceStart = MuzzleLoc;
	
	TriggerFireGameplayCue(MuzzleLoc, BeamTarget, HitResult);

	SendTargetDataToServer(HitResult);
	// 7. 총구 이펙트 (GameplayCue: Muzzle)
	// SocketName 위치에서 Muzzle Flash 재생
}


