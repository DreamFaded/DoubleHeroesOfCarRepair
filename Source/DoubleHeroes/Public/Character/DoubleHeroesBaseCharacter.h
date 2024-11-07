// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DoubleHeroesBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesBaseCharacter();

	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	//Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDHAbilitySystemComponent* DHAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE UDHAbilitySystemComponent* GetDHAbilitySystemComponent() const { return DHAbilitySystemComponent; }

	FORCEINLINE UDoubleHeroesAttributeSet* GetDoubleHeroesAttributeSet() const { return DoubleHeroesAttributeSet; }

};
