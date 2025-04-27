// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/DHAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/DoubleHeroesAbilityTypes.h"
#include "DoubleHeroesGameplayTags.h"
#include "GameMode/DoubleHeroesGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* UDHAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ADoubleHeroesGameMode* DoubleHeroesGameMode = Cast<ADoubleHeroesGameMode>(
		UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return DoubleHeroesGameMode->GetCharacterClassDefaultInfo();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UDHAbilitySystemLibrary::GetCharacterClassDefaultInfo: DoubleHeroesGameMode is nullptr"));
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

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DoubleHeroesGameplayTags::Combat::Data_Damage, DamageEffectInfo.BaseDamage);
	if(IsValid(DamageEffectInfo.TargetASC))
	{
		DamageEffectInfo.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
