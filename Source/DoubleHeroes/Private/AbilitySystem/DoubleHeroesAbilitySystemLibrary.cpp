// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "..\..\Public\DoubleHeroesAbilityTypes.h"
#include "Character/DoubleHeroesCharacter.h"
#include "Game/DoubleHeroesGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/HUD/DoubleHeroesHUD.h"

UOverlayWidgetController* UDoubleHeroesAbilitySystemLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(PC->GetHUD()))
		{
			ADoubleHeroesPlayerState* PS = PC->GetPlayerState<ADoubleHeroesPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DoubleHeroesHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UDoubleHeroesAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(PC->GetHUD()))
		{
			ADoubleHeroesPlayerState* PS = PC->GetPlayerState<ADoubleHeroesPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DoubleHeroesHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UDoubleHeroesAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);

	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UDoubleHeroesAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UDoubleHeroesAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ADoubleHeroesGameModeBase* DoubleHeroesGameMode = Cast<ADoubleHeroesGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(DoubleHeroesGameMode == nullptr) return nullptr;

	return DoubleHeroesGameMode->CharacterClassInfo;
}

bool UDoubleHeroesAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FDoubleHeroesGameplayEffectContext* DoubleHeroesEffectContext = static_cast<const FDoubleHeroesGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DoubleHeroesEffectContext->IsBlockedHit();
	}
	return false;
}

bool UDoubleHeroesAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FDoubleHeroesGameplayEffectContext* DoubleHeroesEffectContext = static_cast<const FDoubleHeroesGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return DoubleHeroesEffectContext->IsCriticalHit();
	}
	return false;
}

void UDoubleHeroesAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsBlockedHit)
{
	if (FDoubleHeroesGameplayEffectContext* DoubleHeroesEffectContext = static_cast<FDoubleHeroesGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DoubleHeroesEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UDoubleHeroesAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FDoubleHeroesGameplayEffectContext* DoubleHeroesEffectContext = static_cast<FDoubleHeroesGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		DoubleHeroesEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

