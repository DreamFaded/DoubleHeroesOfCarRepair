// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueHeroGameplayAbility.h"
#include "DoubleHeroesGameplayAbility.h"
#include "DoubleHeroesGameplayAbility_Jump.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesGameplayAbility_Jump : public UBlueHeroGameplayAbility
{
	GENERATED_BODY()

public:

	UDoubleHeroesGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	void CharacterJumpStop();
};
