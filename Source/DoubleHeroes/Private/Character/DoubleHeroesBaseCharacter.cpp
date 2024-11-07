// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesBaseCharacter.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"


// Sets default values
ADoubleHeroesBaseCharacter::ADoubleHeroesBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	DHAbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("DoubleHeroesAbilitySystemComponent"));

	DoubleHeroesAttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("DoubleHeroesAttributeSet"));
}

UAbilitySystemComponent* ADoubleHeroesBaseCharacter::GetAbilitySystemComponent() const
{
	return GetDHAbilitySystemComponent();
}

void ADoubleHeroesBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (DHAbilitySystemComponent)
	{
		DHAbilitySystemComponent->InitAbilityActorInfo(this, this);

		ensureMsgf(!CharacterStartUpData.IsNull(),TEXT("Forgot to assign start up to %s"), *GetName());
	}
}

