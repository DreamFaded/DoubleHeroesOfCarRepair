// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DoubleHeroesDamageAbility.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/DoubleHeroesAbilityTypes.h"

void UDoubleHeroesDamageAbility::CaptureDamageEffectInfo(AActor* TargetActor, FDamageEffectInfo& OutInfo)
{
	if (AActor* AvatarActorFromInfo = GetAvatarActorFromActorInfo())
	{
		OutInfo.AbilityLevel = GetAbilityLevel();
		OutInfo.AvatarActor = AvatarActorFromInfo;
		OutInfo.BaseDamage = BaseDamage.GetValueAtLevel(GetAbilityLevel());
		OutInfo.DamageEffect = DamageEffect;
		OutInfo.SourceASC = GetDHAbilitySystemComponentFromActorInfo();

		if (IsValid(TargetActor))
		{
			OutInfo.TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
		}
	}
}
