﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DoubleHeroesAbilitySystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDoubleHeroesAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOUBLEHEROES_API IDoubleHeroesAbilitySystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetDynamicSpawnPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel = 1);
};
