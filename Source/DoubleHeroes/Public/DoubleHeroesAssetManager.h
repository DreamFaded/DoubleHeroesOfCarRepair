// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"


#include "DoubleHeroesAssetManager.generated.h"
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UDoubleHeroesAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
