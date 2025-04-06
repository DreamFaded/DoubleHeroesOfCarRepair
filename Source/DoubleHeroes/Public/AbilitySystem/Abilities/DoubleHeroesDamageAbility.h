// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesGameplayAbility.h"
#include "DoubleHeroesDamageAbility.generated.h"

struct FDamageEffectInfo;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesDamageAbility : public UDoubleHeroesGameplayAbility
{
	GENERATED_BODY()

public:
	
	void CaptureDamageEffectInfo(AActor* TargetActor, FDamageEffectInfo& OutInfo);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Damage Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Damage Effect")
	FScalableFloat BaseDamage;
};
