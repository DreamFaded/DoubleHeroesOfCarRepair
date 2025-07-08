// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Character/DoubleHeroesBaseCharacter.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Components/PawnCombatComponent.h"
#include "Player/DoubleHeroesPlayerController.h"

void UDoubleHeroesGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EDoubleHeroesAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UDoubleHeroesGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EDoubleHeroesAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

ADoubleHeroesBaseCharacter* UDoubleHeroesGameplayAbility::GetBaseCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ADoubleHeroesBaseCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

UPawnCombatComponent* UDoubleHeroesGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UDHAbilitySystemComponent* UDoubleHeroesGameplayAbility::GetDHAbilitySystemComponentFromActorInfo() const
{
	return Cast<UDHAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent); 
}

ADoubleHeroesPlayerController* UDoubleHeroesGameplayAbility::GetDoubleHeroesPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ADoubleHeroesPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}
