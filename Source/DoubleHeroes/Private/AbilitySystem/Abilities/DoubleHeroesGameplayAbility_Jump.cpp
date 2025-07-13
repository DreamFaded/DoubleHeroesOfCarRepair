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
	// if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	// {
	// 	return false;
	// }
	//
	// const ABlueHeroCharacter* BlueHeroCharacter = Cast<ABlueHeroCharacter>(ActorInfo->AvatarActor.Get());
	// if (!BlueHeroCharacter || !BlueHeroCharacter->CanJump())
	// {
	// 	return false;
	// }
	//
	// if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	// {
	// 	return false;
	// }
	//
	// return true;
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	return (Character && Character->CanJump());
}

// void UDoubleHeroesGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
// 	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
// 	const FGameplayEventData* TriggerEventData)
// {
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActivateAbility")));
// 	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
// 	{
// 		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
// 		{
// 			return;
// 		}
//
// 		ACharacter * Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
// 		Character->Jump();
// 	}
// }

void UDoubleHeroesGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDoubleHeroesGameplayAbility_Jump::CharacterJumpStart()
{
	if (ABlueHeroCharacter* BlueHeroCharacter = GetHeroCharacterFromActorInfo())
	{
		if (BlueHeroCharacter->IsLocallyControlled() && !BlueHeroCharacter->bPressedJump)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CharacterJumpStart")));
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
