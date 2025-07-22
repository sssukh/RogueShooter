// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/FunctionLibrary_Helper.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "Saves/SG_Player.h"
#include "Utility/RSLog.h"

void UFunctionLibrary_Helper::DamageEnemiesOnce(const UObject* WorldContextObject, const TArray<FHitResult>& EnemyHits,
                                                float Damage, AController* Instigator, AActor* Causer)
{
	TArray<AActor*> HitActors;
	for(const FHitResult& hitResult : EnemyHits)
	{
		AActor* LhitActor = hitResult.GetActor();
		if(LhitActor!=nullptr &&  Damage != 0.f && !HitActors.Contains(LhitActor))
		{
			HitActors.Add(LhitActor);

			// 따로 damageType이 지정되지 않음
			TSubclassOf<UDamageType> const ValidDamageTypeClass =  TSubclassOf<UDamageType>(UDamageType::StaticClass());
			FDamageEvent DamageEvent(ValidDamageTypeClass);
		
			LhitActor->TakeDamage(Damage,DamageEvent,Instigator,Causer);
		}
	}
}

UTexture2D* UFunctionLibrary_Helper::FindActiveIcon(const UObject* WorldContextObject, EActiveAbilities AAbility)
{
	UTexture2D* result = nullptr;
	switch (AAbility)
	{
	case (EActiveAbilities::Hammer):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::HammerDrop);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EActiveAbilities::Frost_Bolt):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::IceSpear);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EActiveAbilities::Lightning):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::Lightning);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EActiveAbilities::Fireball):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::Fireball);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	default:
		break;
	}

	return result;
}

UTexture2D* UFunctionLibrary_Helper::FindPassiveIcon(const UObject* WorldContextObject, EPassiveAbilities PAbility)
{
	UTexture2D* result = nullptr;
	switch (PAbility)
	{
	case (EPassiveAbilities::Ability_Bonus_Damage):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::Upgrade);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EPassiveAbilities::Health_Bonus):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::HealthIncrease);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EPassiveAbilities::Ability_Cooldown_Reduction):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::SpeedoMeter);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	case(EPassiveAbilities::Speed_Bonus):
		{
			ConstructorHelpers::FObjectFinder<UTexture2D> TexFinder(*AssetPath::Texture::Run);
			if(TexFinder.Succeeded())
			{
				result = TexFinder.Object;
			}
			break;
		}
	default:
		break;
	}

	return result;
}

// TODO : WBLoadingScreenWidget 구현 필요 

void UFunctionLibrary_Helper::CreateLoadingScreen(const UObject* WorldContextObject, FText LoadingInfo)
{
	// GetWorld()->GetFirstLocalPlayerFromController()

}

USG_Player* UFunctionLibrary_Helper::LoadPlayerData(const UObject* WorldContextObject)
{
	if(UGameplayStatics::DoesSaveGameExist(FString(TEXT("PlayerData1")),0))
	{
		USaveGame* SG = UGameplayStatics::LoadGameFromSlot(FString(TEXT("PlayerData1")),0);

		if(USG_Player* SGPlayer = Cast<USG_Player>(SG))
		{
			return SGPlayer;
		}

		RS_LOG_ERROR(TEXT("SGPlayer가 USaveGame을 상속받지 않았습니다."))
		return nullptr;
	}
	else
	{
		// TODO : AssetPath에 경로 추가하기
		USG_Player* SG = Cast<USG_Player>(UGameplayStatics::CreateSaveGameObject(StaticLoadClass(USG_Player::StaticClass(),nullptr,*AssetPath::Blueprint::BP_SG_Player_C)));
		
		UDataTable* DT_AvailableCharacter = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,*AssetPath::DataTable::DT_AvailableCharacter));

		if(DT_AvailableCharacter)
		{
			const FAvailableCharacter* RowData = DT_AvailableCharacter->FindRow<FAvailableCharacter>(TEXT("Char1"),TEXT("DT_AvailableCharacter"));
			if(RowData)
			{
				SG->Character = *RowData;
			}
			else
			{
				RS_LOG_ERROR(TEXT("DT_AvailalbeCharacter에서 Char1의 row를 찾지 못했습니다."))
			}
		}
		return SG;
	}
}

void UFunctionLibrary_Helper::SavePlayerData(const UObject* WorldContextObject, USG_Player* SaveData)
{
	UGameplayStatics::SaveGameToSlot(SaveData,TEXT("PlayerData1"),0);
}
