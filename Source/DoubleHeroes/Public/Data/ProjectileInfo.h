﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileInfo.generated.h"

struct FProjectileParams;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UProjectileInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FProjectileParams> ProjectileInfoMap;
};
