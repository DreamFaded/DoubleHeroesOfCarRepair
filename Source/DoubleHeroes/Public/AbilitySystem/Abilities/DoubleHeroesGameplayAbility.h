﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DoubleHeroesGameplayAbility.generated.h"

class UDHAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EDoubleHeroesAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category="DoubleHeroes|Ability")
	EDoubleHeroesAbilityActivationPolicy AbilityActivationPolicy = EDoubleHeroesAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "DoubleHeroes|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category="DoubleHeroes|Ability")
	UDHAbilitySystemComponent* GetDHAbilitySystemComponentFromActorInfo() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
