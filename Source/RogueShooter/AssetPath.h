#pragma once

namespace AssetPath
{
	namespace AI
	{
		const FString BaseEnemyBT = TEXT("BehaviorTree'/Game/RogueShooter/Blueprints/AI/BT_Base_Enemy.BT_Base_Enemy'");

		const FString EQS_FSP = TEXT("EnvQuery'/Game/SurvivorsTemplate/Blueprints/AI/EQS_FindSpawnPoint.EQS_FindSpawnPoint'");
	}

	namespace Animation
	{
		const FString BaseEnemyAnim = TEXT("AnimBlueprint'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/Enemy_Gruntling_Animation/ABP_Gruntling.ABP_Gruntling_C'");
		const FString BaseCharAnim = TEXT("AnimBlueprint'/Game/SurvivorsTemplate/Demo/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'");
	}

	namespace DataTable
	{
		const FString DT_AvailableCharacter = TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_AvailableCharacters.DT_AvailableCharacters'");
		const FString DT_ActiveAbilities = TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_ActiveAbilities.DT_ActiveAbilities'");
		const FString DT_PassiveAbilities = TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_PassiveAbilities.DT_PassiveAbilities'");
		const FString DT_AvailableMaps = TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_AvailableMaps.DT_AvailableMaps'");
		const FString DT_EnemyEliteSpawns= TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_ExampleEliteSpawn.DT_ExampleEliteSpawn'");
		const FString DT_EnemySpawns= TEXT("DataTable'/Game/RogueShooter/Blueprints/DataTables/DT_ExampleSpawn.DT_ExampleSpawn'");


	}
	namespace Curve
	{
		const FString Curve_Soul = TEXT("CurveFloat'/Game/RogueShooter/Resource/SoulCurve.SoulCurve'");
		const FString Curve_Fade = TEXT("CurveFloat'/Game/RogueShooter/Resource/FadeCurve.FadeCurve'");
		const FString Curve_Movement = TEXT("CurveFloat'/Game/RogueShooter/Resource/MovementCurve.MovementCurve'");
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
		const FString Blue = TEXT("MaterialInstanceConstant'/Engine/ArtTools/RenderToTexture/Materials/Debug/M_Emissive_Color_0-0-127.M_Emissive_Color_0-0-127'");
		const FString FireBall = TEXT("Material'/Engine/VREditor/LaserPointer/M_LaserPointer-Core.M_LaserPointer-Core'");

		const FString PotionMaterial = TEXT("Material'/Game/SurvivorsTemplate/Demo/Items/Pickups/PotionPickups/Assets/M_PotionGlass.M_PotionGlass'");

		const FString GoldMaterial = TEXT("Material'/Game/SurvivorsTemplate/Demo/Items/Gold/M_Gold.M_Gold'");

		const FString CubeYellow = TEXT("MaterialInstanceConstant'/Engine/VREditor/BasicMeshes/MI_Cube_01.MI_Cube_01'");
	}

	namespace Mesh
	{
		const FString ChamferCube = TEXT("StaticMesh'/Game/SurvivorsTemplate/Demo/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'");
		const FString BaseEnemyMesh = TEXT("SkeletalMesh'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/SK_Exodus_Gruntling.SK_Exodus_Gruntling'");
		const FString Cube = TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'");

		const FString Potion = TEXT("StaticMesh'/Game/SurvivorsTemplate/Demo/Items/Pickups/PotionPickups/Assets/SM_PotionBottle.SM_PotionBottle'");

		const FString SM_TD_Gold_Pile = TEXT("StaticMesh'/Game/SurvivorsTemplate/Demo/Items/Gold/SM_TD_Gold_Pile.SM_TD_Gold_Pile'");

		const FString SM_Cube = TEXT("StaticMesh'/Engine/VREditor/BasicMeshes/SM_Cube_01.SM_Cube_01'");
	}
	
	namespace Blueprint
	{
		const FString BP_BaseSoul_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/GameplayActors/BP_BaseSoul.BP_BaseSoul_C'");
		const FString BP_FloatingTextActor_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/GameplayActors/BP_FloatingTextActor.BP_FloatingTextActor_C'");
		const FString BP_BaseExplosion_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/Abilities/BP_BaseExplosion.BP_BaseExplosion_C'");
		const FString BP_BaseProjectile_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/Abilities/BP_BaseProjectile.BP_BaseProjectile_C'");
		const FString BP_LightningExplosion_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/Abilities/BP_LightningExplosion.BP_LightningExplosion_C'");
		const FString BP_FireballProjectile_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/Abilities/BP_FireballProjectile.BP_FireballProjectile_C'");

		const FString BP_SG_Player_C = TEXT("Blueprint'/Game/RogueShooter/Blueprints/Saves/BP_SG_Player.BP_SG_Player_C'");

		const FString WBP_FloatingTextWidget_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_FloatingTextWidget.UWB_FloatingTextWidget_C'");
		const FString WBP_HealthBar_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_HealthBar.UWB_HealthBar_C'");
		const FString WBP_PlayerHud_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_PlayerHud.UWB_PlayerHud_C'");
		const FString WBP_LevelUpMaster_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_LevelUpMaster.UWB_LevelUpMaster_C'");
		const FString WBP_MatchResults_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_MatchResults.UWB_MatchResults_C'");
		const FString WBP_ChestMaster_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_ChestMaster.UWB_ChestMaster_C'");
		const FString WBP_LevelUpCard_C = TEXT("");
		const FString WBP_ServerItem_C = TEXT("");
		const FString WBP_ServerBrowser_C = TEXT("");
		const FString WBP_HostMenu_C = TEXT("");
		const FString WBP_InfoPopup_C = TEXT("");
		const FString WBP_LoadingScreen_C = TEXT("");
		const FString WBP_CharacterSelectionItem_C = TEXT("");
		const FString WBP_LobbyMenu_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_LobbyMenu.UWB_LobbyMenu_C'");
		const FString WBP_MainMenu_C = TEXT("WidgetBlueprint'/Game/RogueShooter/Widgets/UWB_MainMenu.UWB_MainMenu_C'");
	}
	namespace Montage
	{
		const FString EnemyMelee = TEXT("AnimMontage'/Game/SurvivorsTemplate/Demo/Animations/AM_EnemyMelee.AM_EnemyMelee'");
		const FString GruntlingDeath = TEXT("AnimMontage'/Game/SurvivorsTemplate/Demo/InfinityBladeAdversaries/Enemy/Enemy_Gruntling/Enemy_Gruntling_Animation/AM_Gruntling_Death.AM_Gruntling_Death'");
		const FString PlayerDeath = TEXT("AnimMontage'/Game/SurvivorsTemplate/Demo/Animations/UE5/AM_PlayerDeath.AM_PlayerDeath'");
	}
	
	namespace Sound
	{
		const FString CueEnemyAttack = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Creatures/Guardian/Cue_EnemyAttack.Cue_EnemyAttack'");
		const FString CueImpact = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Creatures/Guardian/Cue_Impact.Cue_Impact'");
		
		const FString HammerSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Weapons/Hammer/Cue_Hammer.Cue_Hammer'");
		const FString FrostBoltSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Weapons/A_Axe_Swing_Cue.A_Axe_Swing_Cue'");
		const FString LightningSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Abilities/Cue_lightning.Cue_lightning'");
		const FString FireballSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Weapons/A_Axe_Swing_Cue.A_Axe_Swing_Cue'");

		const FString ExplosionSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Abilities/Cue_Explosion.Cue_Explosion'");
		const FString ChestSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Interaction/Cue_Chest.Cue_Chest'");

		const FString CloseMenuSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/UI/Cue_CloseMenu.Cue_CloseMenu'");
		const FString ChestGoldSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Interaction/Cue_ChestGold.Cue_ChestGold'");

		const FString WinSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/UI/Cue_CloseMenu.Cue_CloseMenu'");
		const FString LoseSound = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/UI/Cue_Lose.Cue_Lose'");

		const FString GoldPickup = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Interaction/Cue_GoldPickup.Cue_GoldPickup'");
		const FString HealthPickup = TEXT("SoundCue'/Game/SurvivorsTemplate/Demo/Sounds/Character/Cue_Health.Cue_Health'");
	}
	
	namespace EffectTemplate
	{
		const FString BaseSoul = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Ability/Summon/P_Summon_Parent_Startup.P_Summon_Parent_Startup'");
		const FString BaseEnemy = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Skill_Aura/P_AuraCircle_Default_IronSkin_02.P_AuraCircle_Default_IronSkin_02'");
		
		const FString HammerEffect = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Skill_WeaponThrow/P_Skill_Throw_Fire_Proj_Hammer1.P_Skill_Throw_Fire_Proj_Hammer1'");
		const FString FrostBoltEffect = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Combat_Ice/Impact/P_Impact_Ice.P_Impact_Ice'");
		const FString LightningEffect = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Mobile/Lightning/P_LineToPoint_Blast_Lightning_00.P_LineToPoint_Blast_Lightning_00'");
		const FString FireballEffect = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Monsters/FX_Monster_Gruntling/Bomber/P_FireBombExplosion.P_FireBombExplosion'");

		const FString ImpactIce = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Combat_Ice/Impact/P_Impact_Ice.P_Impact_Ice'");
		const FString Explosion = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Monsters/FX_Monster_Gruntling/Bomber/P_FireBombExplosion.P_FireBombExplosion'");

		const FString DeadMansLoot = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Treasure/Gold_Bag/P_DeadMans_Loot.P_DeadMans_Loot'");
		const FString GoldSparkle = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Treasure/Gold_Bag/P_GoldSparkle_Bag_Pickup.P_GoldSparkle_Bag_Pickup'");
		const FString HealthPickup = TEXT("ParticleSystem'/Game/SurvivorsTemplate/Demo/Effects/FX_Archive/P_HealthOrb_Pickup.P_HealthOrb_Pickup'");
	}
}