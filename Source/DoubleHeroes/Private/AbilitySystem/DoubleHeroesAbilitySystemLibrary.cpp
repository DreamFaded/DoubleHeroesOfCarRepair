// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "DoubleHeroesGameplayTags.h"
#include "LoadScreenSaveGame.h"
#include "AbilitySystem/DoubleHeroesAbilityTypes.h"
#include "Game/DoubleHeroesGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/HUD/DoubleHeroesHUD.h"

bool UDoubleHeroesAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, ADoubleHeroesHUD*& OutDoubleHeroesHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutDoubleHeroesHUD = Cast<ADoubleHeroesHUD>(PC->GetHUD());
		if (OutDoubleHeroesHUD)
		{
			ADoubleHeroesPlayerState* PS = PC->GetPlayerState<ADoubleHeroesPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UDoubleHeroesAbilitySystemLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ADoubleHeroesHUD* DoubleHeroesHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, DoubleHeroesHUD))
	{
		return DoubleHeroesHUD->GetOverlayWidgetController(WCParams);
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

USkillMenuWidgetController* UDoubleHeroesAbilitySystemLibrary::GetSkillMenuWidgetController(
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
			return DoubleHeroesHUD->GetSkillMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UDoubleHeroesAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	// AActor* AvatarActor = ASC->GetAvatarActor();
	//
	// UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	// FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	//
	// FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	// PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	//
	// const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	// ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	//
	// FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	// SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	//
	// const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	// ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	//
	// FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	// VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	//
	// const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	// ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UDoubleHeroesAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;

	const FDoubleHeroesGameplayTags& GameplayTags = FDoubleHeroesGameplayTags::Get();

	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UDoubleHeroesAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
                                                             UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UDoubleHeroesAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ADoubleHeroesGameModeBase* DoubleHeroesGameMode = Cast<ADoubleHeroesGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(DoubleHeroesGameMode == nullptr) return nullptr;

	return DoubleHeroesGameMode->CharacterClassInfo;
}

UAbilityInfo* UDoubleHeroesAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ADoubleHeroesGameModeBase* AuraGameMode = Cast<ADoubleHeroesGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->AbilityInfo;
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

