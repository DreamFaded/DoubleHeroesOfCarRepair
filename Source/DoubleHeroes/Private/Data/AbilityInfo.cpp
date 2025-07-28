// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AbilityInfo.h"

#include "DoubleHeroesLogChannels.h"

FDoubleHeroesAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FDoubleHeroesAbilityInfo& AbilityInfo : AbilityInformation)
	{
		if (AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDoubleHeroes, Error, TEXT("Could not find ability info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), GetNameSafe(this));
	}
	return FDoubleHeroesAbilityInfo();
}
