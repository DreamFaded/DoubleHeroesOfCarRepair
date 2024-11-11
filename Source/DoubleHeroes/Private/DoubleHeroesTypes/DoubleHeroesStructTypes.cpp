// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"

#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility.h"


bool FBlueHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
