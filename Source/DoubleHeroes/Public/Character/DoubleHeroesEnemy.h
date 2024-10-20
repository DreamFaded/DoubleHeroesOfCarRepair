// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesCharacterBase.h"
#include "GameFramework/Character.h"
#include "Interaction/EnemyInterface.h"
#include "DoubleHeroesEnemy.generated.h"

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesEnemy : public ADoubleHeroesCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ADoubleHeroesEnemy();

	//Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;

	virtual int32 GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

private:

	
	
};
