// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AttributeInfo.h"

FDoubleHeroesAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
	bool bLogNotFound) const
{
	for (const FDoubleHeroesAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find info for tag %s"), *AttributeTag.ToString());
	}

	return FDoubleHeroesAttributeInfo();
}
