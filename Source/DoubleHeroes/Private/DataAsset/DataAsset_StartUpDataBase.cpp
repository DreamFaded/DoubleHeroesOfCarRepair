// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_StartUpDataBase.h"

#include "GameplayAbilitySpec.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UDHAbilitySystemComponent* InASCToGive,
                                                              int32 ApplyLevel)
{
	check(InASCToGive)

	GrantAbilities(ActivateOnGiveAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(
	const TArray<TSubclassOf<UDoubleHeroesGameplayAbility>>& InAbilitiesToGive,
	UDHAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UDoubleHeroesGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
