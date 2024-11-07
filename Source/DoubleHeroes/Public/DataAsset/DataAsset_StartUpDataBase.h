// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UDoubleHeroesGameplayAbility;
class UDHAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UDHAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UDoubleHeroesGameplayAbility>> ActivateOnGiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UDoubleHeroesGameplayAbility>> ReactiveAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UDoubleHeroesGameplayAbility>>& InAbilitiesToGive, UDHAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
