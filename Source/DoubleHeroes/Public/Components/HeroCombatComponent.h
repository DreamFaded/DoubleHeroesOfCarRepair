// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"


class ADoubleHeroesWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	ADoubleHeroesWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;


	
};
