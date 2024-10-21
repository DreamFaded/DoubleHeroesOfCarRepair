// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DoubleHeroesInputConfig.h"

const UInputAction* UDoubleHeroesInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FDoubleHeroesInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find ability for input tag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetName());
	}
	return nullptr;
}
