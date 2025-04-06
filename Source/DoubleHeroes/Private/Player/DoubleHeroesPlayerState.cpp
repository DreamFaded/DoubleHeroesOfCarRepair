// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerState.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "..\..\Public\AbilitySystem\DoubleHeroesAttributeSet.h"
#include "Net/UnrealNetwork.h"

ADoubleHeroesPlayerState::ADoubleHeroesPlayerState()
{
	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("AttributeSet"));
	
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

UDHAbilitySystemComponent* ADoubleHeroesPlayerState::GetDHAbilitySystemComponent() const
{
	return Cast<UDHAbilitySystemComponent>(AbilitySystemComponent);
}

void ADoubleHeroesPlayerState::BindCallbacksToDependencies()
{

}

UDoubleHeroesAttributeSet* ADoubleHeroesPlayerState::GetDoubleHeroesAttributes() const
{
	return Cast<UDoubleHeroesAttributeSet>(AttributeSet);
}

void ADoubleHeroesPlayerState::OnRep_Level(int32 OldLevel)
{
}
