// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesBaseAnimInstance.h"
#include "BlueHeroLinkedAnimLayer.generated.h"

class UBlueHeroAnimInstance;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UBlueHeroLinkedAnimLayer : public UDoubleHeroesBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UBlueHeroAnimInstance* GetHeroAnimInstance() const;
};
