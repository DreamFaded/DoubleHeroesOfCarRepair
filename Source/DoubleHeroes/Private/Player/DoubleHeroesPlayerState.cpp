// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerState.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "..\..\Public\AbilitySystem\DoubleHeroesAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADoubleHeroesPlayerState::ADoubleHeroesPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("AttributeSet"));
	
	NetUpdateFrequency = 100.0f;
}

void ADoubleHeroesPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoubleHeroesPlayerState, AbilitySystemComponent);
}

UAbilitySystemComponent* ADoubleHeroesPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADoubleHeroesPlayerState::OnRep_Level(int32 OldLevel)
{
}
