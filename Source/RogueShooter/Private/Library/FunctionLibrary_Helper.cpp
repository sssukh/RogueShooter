// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/FunctionLibrary_Helper.h"

#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "RogueShooter/AssetPath.h"
#include "Saves/SG_Player.h"
#include "System/Subsystem/MyGameInstanceSubsystem.h"
#include "UI/UW_LoadingScreen.h"
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

TSoftObjectPtr<UTexture2D> UFunctionLibrary_Helper::FindActiveIcon(const UObject* WorldContextObject, EActiveAbilities AAbility)
{
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return nullptr;

	UMyGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	if (!MySubsystem) return nullptr ;

	return MySubsystem->GetAAIconFromDB(AAbility);
	
}

TSoftObjectPtr<UTexture2D> UFunctionLibrary_Helper::FindPassiveIcon(const UObject* WorldContextObject, EPassiveAbilities PAbility)
{
	UGameInstance* GameInstance = WorldContextObject->GetWorld()->GetGameInstance();
	if (!GameInstance) return nullptr;

	UMyGameInstanceSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameInstanceSubsystem>();
	if (!MySubsystem) return nullptr ;

	return MySubsystem->GetPAIconFromDB(PAbility);
}

void UFunctionLibrary_Helper::CreateLoadingScreen(const UObject* WorldContextObject, FText LoadingInfo,
	TSubclassOf<UUW_LoadingScreen> LoadingScreenClass)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject,0);

	if(PlayerController->IsLocalController())
	{
		if(UUW_LoadingScreen* LoadingScreen = CreateWidget<UUW_LoadingScreen>(PlayerController,LoadingScreenClass))
		{
			LoadingScreen->Text = LoadingInfo;

			LoadingScreen->AddToViewport();
		}
	}
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
		USG_Player* SG = Cast<USG_Player>(UGameplayStatics::CreateSaveGameObject(StaticLoadClass(USG_Player::StaticClass(),nullptr,*AssetPath::Blueprint::BP_SG_Player_C)));
		
		UDataTable* DT_AvailableCharacter = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(),nullptr,*AssetPath::DataTable::DT_AvailableCharacter));

		if(DT_AvailableCharacter)
		{
			// DT의 row 타입을 바꿔줘야한다.
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


