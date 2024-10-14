// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesCharacterBase.h"
#include "GameFramework/Character.h"
#include "DoubleHeroesCharacter.generated.h"

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesCharacter : public ADoubleHeroesCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActorInfo();
protected:


public:

};
