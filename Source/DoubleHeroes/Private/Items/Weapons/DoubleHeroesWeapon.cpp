// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/DoubleHeroesWeapon.h"


void ADoubleHeroesWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ADoubleHeroesWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
