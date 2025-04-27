// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesDamageAbility.h"
#include "AbilitySystem/DoubleHeroesAbilityTypes.h"
#include "ProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UProjectileAbility : public UDoubleHeroesDamageAbility
{
	GENERATED_BODY()

public:

	UProjectileAbility();

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Projectile")
	FGameplayTag ProjectileToSpawnTag;
private:

	UPROPERTY()
	TObjectPtr<AActor> AvatarActorFromInfo;

	FProjectileParams CurrentProjectileParams;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();
};
