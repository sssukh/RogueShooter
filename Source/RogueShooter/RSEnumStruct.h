#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ParticleHelper.h"
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


