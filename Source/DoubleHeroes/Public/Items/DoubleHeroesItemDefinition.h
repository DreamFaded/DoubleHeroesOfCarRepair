// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DoubleHeroesItemDefinition.generated.h"

class AActor;
class UDoubleHeroesAbilitySet;
class UDoubleHeroesItemInstance;

USTRUCT()
struct FDoubleHeroesItemActorToSpawn
{
	GENERATED_BODY()

	FDoubleHeroesItemActorToSpawn()
	{}

	UPROPERTY(EditAnywhere, Category=Item)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Item)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Item)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class UDoubleHeroesItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	
	UDoubleHeroesItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Class to spawn
	UPROPERTY(EditDefaultsOnly, Category=Item)
	TSubclassOf<UDoubleHeroesItemInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Item)
	TArray<TObjectPtr<const UDoubleHeroesAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(EditDefaultsOnly, Category=Item)
	TArray<FDoubleHeroesItemActorToSpawn> ActorsToSpawn;
};
