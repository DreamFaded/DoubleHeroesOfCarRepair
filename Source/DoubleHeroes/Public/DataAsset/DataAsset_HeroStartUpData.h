// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartUpDataBase.h"
#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"
#include "DataAsset_HeroStartUpData.generated.h"
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UDHAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FBlueHeroAbilitySet> HeroStartUpAbilitySets;
};
