// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EquipmentInstance.generated.h"

struct FEquipmentActorsToSpawn;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual void OnEquipped();
	virtual void OnUnequipped();

	void SpawnEquipmentActors(const TArray<FEquipmentActorsToSpawn>& ActorToSpawns);
	void DestroySpawnedActors();


private:
	UPROPERTY()
	TArray<AActor*> SpawnedActors;

	ACharacter* GetCharacter();
};
