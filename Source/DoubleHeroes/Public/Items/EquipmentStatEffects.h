// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentStatEffects.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UEquipmentStatEffects : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TObjectPtr<UDataTable>> MasterStatMap;
};
