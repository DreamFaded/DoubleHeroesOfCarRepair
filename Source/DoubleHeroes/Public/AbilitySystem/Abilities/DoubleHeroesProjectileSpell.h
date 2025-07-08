// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesDamageGameplayAbility.h"
#include "DoubleHeroesProjectileSpell.generated.h"

class UGameplayEffect;
class ADoubleHeroesProjectile;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesProjectileSpell : public UDoubleHeroesDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADoubleHeroesProjectile> ProjectileClass;

};
