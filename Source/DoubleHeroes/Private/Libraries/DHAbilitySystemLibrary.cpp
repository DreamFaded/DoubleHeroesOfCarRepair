// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/DHAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "DoubleHeroesAbilityTypes.h"
#include "GameMode/DoubleHeroesGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* UDHAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ADoubleHeroesGameMode* DoubleHeroesGameMode = Cast<ADoubleHeroesGameMode>(
		UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return DoubleHeroesGameMode->GetCharacterClassDefaultInfo();
	}
	return nullptr;
}

UProjectileInfo* UDHAbilitySystemLibrary::GetProjectileInfo(const UObject* WorldContextObject)
{
	if (const ADoubleHeroesGameMode* DoubleHeroesGameMode = Cast<ADoubleHeroesGameMode>(
		UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return DoubleHeroesGameMode->GetProjectileInfo();
	}
	return nullptr;
}

void UDHAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectInfo& DamageEffectInfo)
{
	FGameplayEffectContextHandle ContextHandle = DamageEffectInfo.SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(DamageEffectInfo.AvatarActor);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectInfo.SourceASC->MakeOutgoingSpec(DamageEffectInfo.DamageEffect, DamageEffectInfo.AbilityLevel, ContextHandle);

	if(IsValid(DamageEffectInfo.TargetASC))
	{
		DamageEffectInfo.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
