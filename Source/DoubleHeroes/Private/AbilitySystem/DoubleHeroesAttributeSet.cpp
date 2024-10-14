#include "..\..\Public\AbilitySystem\DoubleHeroesAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UDoubleHeroesAttributeSet::UDoubleHeroesAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitEndurance(50.f);
	InitMaxEndurance(100.f);
}

void UDoubleHeroesAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDoubleHeroesAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDoubleHeroesAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDoubleHeroesAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDoubleHeroesAttributeSet, MaxEndurance, COND_None, REPNOTIFY_Always);
}

void UDoubleHeroesAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDoubleHeroesAttributeSet, Health, OldHealth);
}

void UDoubleHeroesAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDoubleHeroesAttributeSet, MaxHealth, OldMaxHealth);
}

void UDoubleHeroesAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDoubleHeroesAttributeSet, Endurance, OldEndurance);
}

void UDoubleHeroesAttributeSet::OnRep_MaxEndurance(const FGameplayAttributeData& OldMaxEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDoubleHeroesAttributeSet, MaxEndurance, OldMaxEndurance);
}
