#pragma once

namespace AssetPath
{
	namespace Curve
	{
		const FString Curve_Soul = TEXT("CurveFloat'/Game/SurvivorsTemplate/Resource/SoulCurve.SoulCurve'");
	}

	namespace Texture
	{
		const FString Fireball = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/fireball.fireball'");
		const FString Lightning = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/focused-lightning.focused-lightning'");
		const FString HammerDrop = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/hammer-drop.hammer-drop'");
		const FString HealthIncrease = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/health-increase.health-increase'");
		const FString HealthPotion = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/health-potion.health-potion'");
		const FString IceSpear = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/ice-spear.ice-spear'");
		const FString Run = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/run.run'");
		const FString SpeedoMeter = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/speedometer.speedometer'");
		const FString TwoCoins = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/two-coins.two-coins'");
		const FString Upgrade = TEXT("Texture2D'/Game/SurvivorsTemplate/Textures/upgrade.upgrade'");
	}

	namespace Material
	{
		
	}

	namespace Montage
	{
		const FString EnemyMelee = TEXT("AnimMontage'/Game/SurvivorsTemplate/Demo/Animations/AM_EnemyMelee.AM_EnemyMelee'");
		const FString GruntlingDeath = TEXT("AnimMontage'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/Enemy_Gruntling_Animation/AM_Gruntling_Death.AM_Gruntling_Death'");
	}
	
	namespace Sound
	{
		const FString CueEnemyAttack = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Creatures/Guardian/Cue_EnemyAttack.Cue_EnemyAttack'");
		const FString CueImpact = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Creatures/Guardian/Cue_Impact.Cue_Impact'");
	}
	
	namespace EffectTemplate
	{
		const FString BaseSoul = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Ability/Summon/P_Summon_Parent_Startup.P_Summon_Parent_Startup'");
		const FString BaseEnemy = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Skill_Aura/P_AuraCircle_Default_IronSkin_02.P_AuraCircle_Default_IronSkin_02'");
		
	}
}