// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Subsystem/MyGameInstanceSubsystem.h"

#include "Data/AAbilitySet.h"
#include "Data/PAbilitySet.h"
#include "RogueShooter/AssetPath.h"
#include "Utility/RSLog.h"


UMyGameInstanceSubsystem::UMyGameInstanceSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UAAbilitySet> AAbilityFinder(*AssetPath::DataAsset::DA_AAbilityIcons);

	if(AAbilityFinder.Succeeded())
	{
		AAbilityIconDB = AAbilityFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPAbilitySet> PAbilityFinder(*AssetPath::DataAsset::DA_PAbilityIcons);

	if(PAbilityFinder.Succeeded())
	{
		PAbilityIconDB = PAbilityFinder.Object;
	}
}

TSoftObjectPtr<UTexture2D> UMyGameInstanceSubsystem::GetAAIconFromDB(EActiveAbilities AAbility)
{
	if(AAbilityIconDB)
	{
		if(const TSoftObjectPtr<UTexture2D>* FoundPath = AAbilityIconDB->AAbilityIcon.Find(AAbility))
		{
			return *FoundPath;
		}
	}
	
	RS_LOG_WARNING(TEXT("Fail to find Active Ability Icon"))
	return nullptr;
}

TSoftObjectPtr<UTexture2D> UMyGameInstanceSubsystem::GetPAIconFromDB(EPassiveAbilities PAbility)
{
	if(PAbilityIconDB)
	{
		if(const TSoftObjectPtr<UTexture2D>* FoundPath = PAbilityIconDB->PAbilityIcon.Find(PAbility))
		{
			return *FoundPath;
		}
	}

	RS_LOG_WARNING(TEXT("Fail to find Passive Ability Icon"))
	return nullptr;
}
