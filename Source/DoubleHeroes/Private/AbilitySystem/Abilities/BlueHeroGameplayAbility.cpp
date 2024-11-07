// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BlueHeroGameplayAbility.h"

#include "Character/BlueHeroCharacter.h"
#include "Player/DoubleHeroesPlayerController.h"

ABlueHeroCharacter* UBlueHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedBlueHeroCharacter.IsValid())
	{
	CachedBlueHeroCharacter = Cast<ABlueHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedBlueHeroCharacter.IsValid()? CachedBlueHeroCharacter.Get() : nullptr;
}

ADoubleHeroesPlayerController* UBlueHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedBlueHeroController.IsValid())
	{
		CachedBlueHeroController = Cast<ADoubleHeroesPlayerController>(CurrentActorInfo->PlayerController);
	}
	return CachedBlueHeroCharacter.IsValid() ? CachedBlueHeroController.Get() : nullptr;
}

UHeroCombatComponent* UBlueHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
