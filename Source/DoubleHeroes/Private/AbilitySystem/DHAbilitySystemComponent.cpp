// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DHAbilitySystemComponent.h"

void UDHAbilitySystemComponent::AbilityActorInfoSet()
{
		OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDHAbilitySystemComponent::EffectApplied);
}

void UDHAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
