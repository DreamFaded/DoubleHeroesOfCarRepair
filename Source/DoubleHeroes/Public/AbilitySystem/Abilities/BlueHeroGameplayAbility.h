// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesGameplayAbility.h"
#include "BlueHeroGameplayAbility.generated.h"

class UHeroCombatComponent;
class ADoubleHeroesPlayerController;
class ABlueHeroCharacter;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UBlueHeroGameplayAbility : public UDoubleHeroesGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DoubleHeroes|Ability")
	ABlueHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "DoubleHeroes|Ability")
	ADoubleHeroesPlayerController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "DoubleHeroes|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<ABlueHeroCharacter> CachedBlueHeroCharacter;
	TWeakObjectPtr<ADoubleHeroesPlayerController> CachedBlueHeroController;
};
