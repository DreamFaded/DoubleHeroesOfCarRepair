// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DoubleHeroesAbilitySystemGlobals.h"

#include "DoubleHeroesAbilityTypes.h"

FGameplayEffectContext* UDoubleHeroesAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDoubleHeroesGameplayEffectContext();
}
