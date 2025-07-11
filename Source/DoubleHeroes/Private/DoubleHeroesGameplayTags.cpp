﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleHeroesGameplayTags.h"

#include "GameplayTagsManager.h"

namespace DoubleHeroesGameplayTags::Combat
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage, "Combat.Data.Damage", "Set By Caller Data Tag For Combat");

}

namespace DoubleHeroesGameplayTags::Player::State
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Crouch, "Player.State.Crouch", "Player Is Actively Crouching");
}

namespace DoubleHeroesGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_StopMove, "InputTag.StopMove");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveForward, "InputTag.MoveForward");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveBackward, "InputTag.MoveBackward");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveLeft, "InputTag.MoveLeft");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveRight, "InputTag.MoveRight");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Run, "InputTag.Run");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_TogglePackage, "InputTag.TogglePackage");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipPipe, "InputTag.EquipPipe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipPipe, "InputTag.UnequipPipe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_FireBowl, "InputTag.LightAttack.FireBowl");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_FireBowl, "InputTag.HeavyAttack.FireBowl");
	//Player Tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_FireBowl, "Player.Ability.Equip.FireBowl");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_FireBowl, "Player.Ability.Unequip.FireBowl");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_FireBowl, "Player.Ability.Attack.Light.FireBowl");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_FireBowl, "Player.Ability.Attack.Heavy.FireBowl");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_FireBowl, "Player.Weapon.FireBowl");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_FireBowl, "Player.Event.Equip.FireBowl");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_FireBowl, "Player.Event.Unequip.FireBowl");
}

FDoubleHeroesGameplayTags FDoubleHeroesGameplayTags::GameplayTags;

void FDoubleHeroesGameplayTags::InitializeNativeGameplayTags()
{
	//Primary Attributes
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), FString("Increases magical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), FString("Increases Armor and Armor Penetration"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("Increases Health"));

	//Secondary Attributes
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), FString("Reduce damage taken, improves Block Chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignore percentage of enemy Armor, increases Critical Hit Chance"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage added when a critical hit is scored"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_EnduranceRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.EnduranceRegeneration"), FString("Amount of Endurance regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxEndurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxEndurance"), FString("Maximum amount of Endurance obtainable"));


	//Input Tags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"), FString("Input Tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"), FString("Input Tag for Right Mouse Button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"), FString("Input Tag for 1 key"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.2"), FString("Input Tag for 2 key"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.3"), FString("Input Tag for 3 key"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.4"), FString("Input Tag for 4 key"));
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"), FString("Fire Damage Type"));
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage"));
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage"));

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects_HitReact"), FString("Tag granted when Hit Reacting"));



	//Abilities
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("Attack Ability Tag"));
}
