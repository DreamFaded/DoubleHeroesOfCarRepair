// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesCharacterBase.h"
#include "DoubleHeroesCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesCharacter : public ADoubleHeroesCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;

private:
	virtual void InitAbilityActorInfo() override;
protected:


public:

};
