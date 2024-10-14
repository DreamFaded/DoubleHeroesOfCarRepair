// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerState.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "..\..\Public\AbilitySystem\DoubleHeroesAttributeSet.h"

ADoubleHeroesPlayerState::ADoubleHeroesPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("AttributeSet"));
	
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ADoubleHeroesPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
