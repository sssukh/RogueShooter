#pragma once

namespace AssetPath
{
	namespace AI
	{
		const FString BaseEnemyBT = TEXT("BehaviorTree'/Game/RogueShooter/Blueprints/AI/BT_Base_Enemy.BT_Base_Enemy'");	
	}

	namespace Animation
	{
		const FString BaseEnemyAnim = TEXT("AnimBlueprint'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/Enemy_Gruntling_Animation/ABP_Gruntling.ABP_Gruntling_C'");		
	}
	namespace Curve
	{
		const FString Curve_Soul = TEXT("CurveFloat'/Game/SurvivorsTemplate/Resource/SoulCurve.SoulCurve'");
		const FString Curve_Fade = TEXT("CurveFloat'/Game/SurvivorsTemplate/Resource/FadeCurve.FadeCurve'");
		const FString Curve_Movement = TEXT("CurveFloat'/Game/SurvivorsTemplate/Resource/MovementCurve.MovementCurve'");
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
		const FString BaseChestMat = TEXT("MaterialInstanceConstant'/Engine/VREditor/LaserPointer/LaserPointerMaterialInst.LaserPointerMaterialInst'");
	}

	namespace Mesh
	{
		const FString ChamferCube = TEXT("StaticMesh'/Game/SurvivorsTemplate/Demo/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'");
		const FString BaseEnemyMesh = TEXT("SkeletalMesh'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/SK_Exodus_Gruntling.SK_Exodus_Gruntling'");
	}
	
	namespace Blueprint
	{
		const FString WB_FloatingTextWidget_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/WB_FloatingTextWidget.WB_FloatingTextWidget_C'");
		const FString BP_BaseSoul_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/GameplayActors/BP_BaseSoul.BP_BaseSoul_C'");
		const FString BP_FloatingTextActor_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/GameplayActors/BP_FloatingTextActor.BP_FloatingTextActor_C'");
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