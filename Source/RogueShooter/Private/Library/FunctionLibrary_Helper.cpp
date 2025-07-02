// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/FunctionLibrary_Helper.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

void UFunctionLibrary_Helper::DamageEnemiesOnce(const TArray<FHitResult>& EnemyHits, float Damage,
                                                AController* Instigator, AActor* Causer, const TSubclassOf<UDamageType> DamageTypeClass)
{
	TArray<AActor*> HitActors;
	for(const FHitResult& hitResult : EnemyHits)
	{
		AActor* LhitActor = hitResult.GetActor();
		if(LhitActor!=nullptr &&  Damage != 0.f && !HitActors.Contains(LhitActor))
		{
			HitActors.Add(LhitActor);

			// 따로 damageType이 지정되지 않음
			TSubclassOf<UDamageType> const ValidDamageTypeClass = DamageTypeClass ? DamageTypeClass : TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);
		
			LhitActor->TakeDamage(Damage,DamageEvent,Instigator,Causer);
		}
	}
}

// TODO : 텍스처들 코드에 업로드해두자.
UTexture2D* UFunctionLibrary_Helper::FindActiveIcon(EActiveAbilities AAbility)
{
	UTexture2D* result = nullptr;
	switch (AAbility)
	{
		case (EActiveAbilities::Hammer):
			{
				break;
			}
		case(EActiveAbilities::Frost_Bolt):
			{
				break;
			}
		case(EActiveAbilities::Lightning):
			{
				break;
			}
		case(EActiveAbilities::Fireball):
			{
				break;
			}
		default:
			break;
	}

	return result;
}

UTexture2D* UFunctionLibrary_Helper::FindPassiveIcon(EPassiveAbilities PAbility)
{
	UTexture2D* result = nullptr;
	switch (PAbility)
	{
	case (EPassiveAbilities::Ability_Bonus_Damage):
		{
			break;
		}
	case(EPassiveAbilities::Health_Bonus):
		{
			break;
		}
	case(EPassiveAbilities::Ability_Cooldown_Reduction):
		{
			break;
		}
	case(EPassiveAbilities::Speed_Bonus):
		{
			break;
		}
	default:
		break;
	}

	return result;
}

// TODO : WBLoadingScreenWidget 구현 필요 
void UFunctionLibrary_Helper::CreateLoadingScreen(FText LoadingInfo)
{
	// GetWorld()->GetFirstLocalPlayerFromController()
}
