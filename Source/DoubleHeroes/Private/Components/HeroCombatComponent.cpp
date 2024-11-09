// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HeroCombatComponent.h"

#include "Items/Weapons/DoubleHeroesWeapon.h"

ADoubleHeroesWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADoubleHeroesWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
