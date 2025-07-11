﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"


namespace DoubleHeroesGameplayTags::Combat
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
}

namespace DoubleHeroesGameplayTags::Player::State
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Crouch);
}

namespace DoubleHeroesGameplayTags
{
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_StopMove);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MoveForward);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MoveBackward);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MoveLeft);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MoveRight);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Run);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_TogglePackage);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipPipe);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipPipe);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_FireBowl);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_FireBowl);

	//Player Tags
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_FireBowl);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_FireBowl);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_FireBowl);
	DOUBLEHEROES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_FireBowl);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_FireBowl);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_FireBowl);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_FireBowl);
}
/**
 * DoubleHeroesGameplayTags
 *
 * Singleton containing native gameplay tags
 */
struct FDoubleHeroesGameplayTags
{
public:
	static const FDoubleHeroesGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxEndurance;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_EnduranceRegeneration;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	TArray<FGameplayTag> DamageTypes;
	
	FGameplayTag Abilities_Attack;


	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	FGameplayTag Effects_HitReact;
	
private:
	static FDoubleHeroesGameplayTags GameplayTags;
};
