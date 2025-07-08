// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility_Jump.h"

#include "Character/BlueHeroCharacter.h"

UDoubleHeroesGameplayAbility_Jump::UDoubleHeroesGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UDoubleHeroesGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ABlueHeroCharacter* BlueHeroCharacter = Cast<ABlueHeroCharacter>(ActorInfo->AvatarActor.Get());
	if (!BlueHeroCharacter || !BlueHeroCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UDoubleHeroesGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDoubleHeroesGameplayAbility_Jump::CharacterJumpStart()
{
	if (ABlueHeroCharacter* BlueHeroCharacter = GetHeroCharacterFromActorInfo())
	{
		if (BlueHeroCharacter->IsLocallyControlled() && !BlueHeroCharacter->bPressedJump)
		{
			BlueHeroCharacter->UnCrouch();
			BlueHeroCharacter->Jump();
		}
	}
}

void UDoubleHeroesGameplayAbility_Jump::CharacterJumpStop()
{
	if (ABlueHeroCharacter* BlueHeroCharacter = GetHeroCharacterFromActorInfo())
	{
		if (BlueHeroCharacter->IsLocallyControlled() && BlueHeroCharacter->bPressedJump)
		{
			BlueHeroCharacter->StopJumping();
		}
	}
}
