﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesWeaponBase.h"
#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"
#include "DoubleHeroesWeapon.generated.h"


UCLASS()
class DOUBLEHEROES_API ADoubleHeroesWeapon : public ADoubleHeroesWeaponBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FDoubleHeroesWeaponData HeroesWeaponData;
};