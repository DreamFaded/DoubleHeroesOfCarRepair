﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class ADoubleHeroesWeaponBase;
struct FGameplayTag;

UCLASS()
class DOUBLEHEROES_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ADoubleHeroesWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	ADoubleHeroesWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "DoubleHeroes|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	ADoubleHeroesWeaponBase* GetCharacterCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, ADoubleHeroesWeaponBase*> CharacterCarriedWeaponMap;
};