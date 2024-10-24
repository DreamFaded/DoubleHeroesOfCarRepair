// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DHAbilitySystemComponent.h"

#include "DoubleHeroesGameplayTags.h"
#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility.h"

void UDHAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDHAbilitySystemComponent::ClientEffectApplied);
}

void UDHAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UDoubleHeroesGameplayAbility* DoubleHeroesAbility = Cast<UDoubleHeroesGameplayAbility>(
			AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(DoubleHeroesAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UDHAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UDHAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UDHAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec,
                                              FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	//WidgetController会监听这个
	EffectAssetTags.Broadcast(TagContainer);
}
