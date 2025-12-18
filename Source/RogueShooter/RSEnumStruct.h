#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "ParticleHelper.h"
#include "Data/Item/ItemData.h"
#include "RSEnumStruct.generated.h"

class ABase_Enemy;
class ASoul;

UENUM(BlueprintType)
enum class EActiveAbilities : uint8
{
	Hammer,
	FrostBolt,
	Lightning,
	Fireball,
	MAX
};

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Active,
	Passive,
	Gold,
	Health,
	Evolution
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Lena_Dyere,
	Osborn_Winfielde,
	Eadwulf_Fury
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Active,
	Disabled
};

UENUM(BlueprintType)
enum class EPassiveAbilities : uint8
{
	Ability_Bonus_Damage,
	Health_Bonus,
	Ability_Cooldown_Reduction,
	Speed_Bonus,
	MAX
};

USTRUCT(BlueprintType)
struct FAbilityLevelUp : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ability LevelUp")
	FText LevelUpText;

	FAbilityLevelUp()
	{
		LevelUpText = FText::FromString(TEXT("What does ability do?"));
	}
};

USTRUCT(BlueprintType)
struct FAvailableCharacter :public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	EActiveAbilities StartingAbilities;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	TObjectPtr<USkeletalMesh> CharacterSK;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	bool DefaultLocked;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	int32 UnlockCost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Character")
	FText CharacterName;

	FAvailableCharacter()
	{
		StartingAbilities = EActiveAbilities::Hammer;

		DefaultLocked = false;

		UnlockCost = 0;

		Description = FText::FromString(TEXT("Description of Character"));
		
		CharacterName = FText::FromString(TEXT("Arlo"));
	}
};

USTRUCT(BlueprintType)
struct FAvailableMaps : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Maps")
	FName MapName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Available Maps")
	FText Description;
};

USTRUCT(BlueprintType)
struct FEnemySpawnType : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	TSubclassOf<ABase_Enemy> Enemy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	int32 AllowedWave = 0 ;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	float Health = 15.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	TSubclassOf<ASoul> Soul;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	bool IsElite = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawn Type")
	bool IsScaleHptoLevel = false;

	FEnemySpawnType()
	{
		
	}
};

USTRUCT(BlueprintType)
struct FEnemySpawns
{
	GENERATED_BODY()

	FEnemySpawns(){Spawns = TArray<FEnemySpawnType>();}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Enemy Spawns")
	TArray<FEnemySpawnType> Spawns;
	
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
public:
	FInventorySlot(){};
	
	explicit FInventorySlot(UItemData* Item)
	{
		ItemData = Item;
		Count = 1;
	}
	
	explicit FInventorySlot(UItemData* Item, int32 ItemAmount)
	{
		ItemData = Item;
		Count = ItemAmount;
	}
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemData* ItemData = nullptr; // 어떤 아이템인지 (DataAsset 포인터)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0; // 몇 개인지 (상태 값)

	// 스탯 효과 GAS 이펙트 핸들
	FActiveGameplayEffectHandle ActivePassiveEffectHandle;
	
	// 로직 능력 핸들
	FGameplayAbilitySpecHandle ActiveAbilitySpecHandle;
	
	// 빈 슬롯인지 확인하는 헬퍼 함수
	bool IsEmpty() const { return !ItemData || Count <= 0; }
};

USTRUCT(BlueprintType)
struct FStatUIData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// [키] 이 UI 정보가 어떤 태그에 대한 것인가? (예: Stat.Attack)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StatTag;

	// [이름] 화면에 표시할 이름 (예: "공격력")
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	// [아이콘] 스탯 아이콘 (옵션)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon = nullptr;

	// [서식] 표시 형식 (예: "{0} +{1}")
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText FormatString; // 기본값: NSLOCTEXT("UI", "DefaultStat", "{0}: {1}")
};