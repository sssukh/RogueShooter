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

UENUM(BlueprintType)
enum class EDamageReceiveType : uint8
{
	Default,
	Health,
	Shield
};

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Attack,
	// 왼쪽 마우스
	Skill1, 
	// 오른쪽 마우스
	Skill2, 
	// shift
	Skill3, 
	// r
	Skill4, 
	// q
	Skill5, 
};

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 스킬 키값 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag AbilityTag;
	
	// 스킬에 사용할 아이콘 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UTexture2D> SkillIcon;
	
	// 스킬 설명 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText SkillDescription;
	
	// 스킬 이름 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText DisplayName;
};

USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
	GENERATED_BODY()

	// 스폰할 몬스터 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Data")
	TSubclassOf<ABase_Enemy> MonsterClass;

	// 해당 몬스터의 스폰 마릿수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Data")
	int32 SpawnCount = 10;

	// 해당 몬스터가 스폰되는 간격 (초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Data")
	float SpawnInterval = 1.0f;
};

USTRUCT(BlueprintType)
struct FWaveInfo : public FTableRowBase
{
	GENERATED_BODY()

	// 이번 웨이브에 등장할 몬스터 그룹 배열 (여러 종류 스폰 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Data")
	TArray<FMonsterSpawnInfo> SpawnGroups;

	// 웨이브 시작 전 대기 시간 (상점 이용, 스킬 선택 등을 위한 시간)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave Data")
	float DelayBeforeStart = 5.0f;
};

// UENUM(BlueprintType)
// enum class EWaveState : uint8
// {
// 	NotStarted,   // 시작 전 (초기 상태)
// 	Preparing,    // 웨이브 시작 전 대기 (상점/스킬 선택 등)
// 	Spawning,     // 몬스터 생성 중
// 	Combat,       // 생성 완료, 남은 적 섬멸 중
// 	WaveCleared,  // 웨이브 클리어
// 	GameOver      // 플레이어 사망 등 게임 오버
// };