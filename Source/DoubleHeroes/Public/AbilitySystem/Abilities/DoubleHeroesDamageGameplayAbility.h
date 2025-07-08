// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesGameplayAbility.h"
#include "DoubleHeroesDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesDamageGameplayAbility : public UDoubleHeroesGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
