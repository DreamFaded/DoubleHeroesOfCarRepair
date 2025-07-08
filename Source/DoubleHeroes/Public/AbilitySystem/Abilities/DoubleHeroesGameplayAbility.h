// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DoubleHeroesGameplayAbility.generated.h"

class ADoubleHeroesPlayerController;
class UDHAbilitySystemComponent;
class UPawnCombatComponent;
class ADoubleHeroesBaseCharacter;

UENUM(BlueprintType)
enum class EDoubleHeroesAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven,
	OnSpawn
};
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Values|Input")
	FGameplayTag InputTag;
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category="DoubleHeroes|Ability")
	EDoubleHeroesAbilityActivationPolicy AbilityActivationPolicy = EDoubleHeroesAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability")
	ADoubleHeroesBaseCharacter* GetBaseCharacterFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "DoubleHeroes|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category="DoubleHeroes|Ability")
	UDHAbilitySystemComponent* GetDHAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability")
	ADoubleHeroesPlayerController* GetDoubleHeroesPlayerControllerFromActorInfo() const;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DoubleHeroes|Ability Activation")
	EDoubleHeroesAbilityActivationPolicy ActivationPolicy;


};
