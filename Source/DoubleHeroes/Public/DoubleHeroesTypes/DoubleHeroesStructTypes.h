// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesStructTypes.generated.h"

class UBlueHeroLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FDoubleHeroesWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBlueHeroLinkedAnimLayer> WeaponAnimLayerToLink;
};