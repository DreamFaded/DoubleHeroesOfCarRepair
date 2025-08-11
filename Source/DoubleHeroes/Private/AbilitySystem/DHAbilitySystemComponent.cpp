// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DHAbilitySystemComponent.h"

#include "DoubleHeroesGameplayTags.h"
#include "DoubleHeroesLogChannels.h"
#include "LoadScreenSaveGame.h"
#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility.h"
#include "AbilitySystem/Abilities/ProjectileAbility.h"
#include "Data/AbilityInfo.h"
#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Items/EquipmentManagerComponent.h"

void UDHAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDHAbilitySystemComponent::ClientEffectApplied);
}

void UDHAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
	for (const FSavedAbility& Data : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> LoadedAbilityClass = Data.GameplayAbility;

		FGameplayAbilitySpec LoadedAbilitySpec = FGameplayAbilitySpec(LoadedAbilityClass, Data.AbilityLevel);

		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilitySlot);
		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilityStatus);
		if (Data.AbilityType == FDoubleHeroesGameplayTags::Get().Abilities_Type_Offensive)
		{
			GiveAbility(LoadedAbilitySpec);
		}
		else if (Data.AbilityType == FDoubleHeroesGameplayTags::Get().Abilities_Type_Passive)
		{
			if (Data.AbilityStatus.MatchesTagExact(FDoubleHeroesGameplayTags::Get().Abilities_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(LoadedAbilitySpec);
				MulticastActivatePassiveEffect(Data.AbilityTag, true);
			}
			else
			{
				GiveAbility(LoadedAbilitySpec);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UDHAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	// if (!InputTag.IsValid())
	// {
	// 	return;
	// }
	// for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	// {
	// 	if(!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
	//
	// 	TryActivateAbility(AbilitySpec.Handle);
	// }

	//GAS
	if(!InputTag.IsValid()) return;
	
	ABILITYLIST_SCOPE_LOCK();
	
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
			else
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle,
					Spec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
	//GAS

	//Lyra
	// if (InputTag.IsValid())
	// {
	// 	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	// 	{
	// 		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
	// 		{
	// 			InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
	// 			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
	// 		}
	// 	}
	// }
}

void UDHAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	
	ABILITYLIST_SCOPE_LOCK();
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
				Spec.ActivationInfo.GetActivationPredictionKey());
		}
	}
	
	// if (InputTag.IsValid())
	// {
	// 	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	// 	{
	// 		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
	// 		{
	// 			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
	// 			InputHeldSpecHandles.Remove(AbilitySpec.Handle);
	// 		}
	// 	}
	// }
}

/*void UDHAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UDoubleHeroesGameplayAbility* DoubleHeroesAbility = Cast<UDoubleHeroesGameplayAbility>(
			AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(DoubleHeroesAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}*/

void UDHAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UDHAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// if (!InputTag.IsValid()) return;
	//
	// for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	// {
	// 	if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
	// 	{
	// 		AbilitySpecInputReleased(AbilitySpec);
	// 	}
	// }

	//GAS
	if(!InputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact((InputTag)))
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle,
			                      Spec.ActivationInfo.GetActivationPredictionKey());
			
		}
	}
}

void UDHAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLockk(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogDoubleHeroes, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__)
		}
	}
}

FGameplayTag UDHAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UDHAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UDHAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

void UDHAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

FGameplayAbilitySpec* UDHAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UDHAbilitySystemComponent::SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel)
{
	if(!ProjectileTag.IsValid()) return;

	if (!GetAvatarActor()->HasAuthority())
	{
		ServerSetDynamicProjectile(ProjectileTag, AbilityLevel);
		return;
	}

	if (ActiveProjectileAbility.IsValid())
	{
		ClearAbility(ActiveProjectileAbility);
	}

	if (IsValid(DynamicProjectileAbility))
	{
		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(DynamicProjectileAbility, AbilityLevel);
		if(UProjectileAbility* ProjectileAbility = Cast<UProjectileAbility>(Spec.Ability))
		{
			ProjectileAbility->ProjectileToSpawnTag = ProjectileTag;
			// Spec.DynamicAbilityTags.AddTag(ProjectileAbility->InputTag);
			ActiveProjectileAbility = GiveAbility(Spec);
		}
	}
}

void UDHAbilitySystemComponent::AddEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry)
{
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	TWeakObjectPtr<UDHAbilitySystemComponent> WeakThis(this);
	
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	
	for (const FEquipmentStatEffectGroup& StatEffect : EquipmentEntry->EffectPackage.StatEffects)
	{
		if (IsValid(StatEffect.EffectClass.Get()))
		{
			const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(StatEffect.EffectClass.Get(), StatEffect.CurrentValue, ContextHandle);
			const FActiveGameplayEffectHandle ActiveHandle = ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			EquipmentEntry->GrantedHandles.AddEffectHandle(ActiveHandle);
		}
		else
		{
			Manager.RequestAsyncLoad(StatEffect.EffectClass.ToSoftObjectPath(),
				[WeakThis, StatEffect, ContextHandle, EquipmentEntry]
				{
					const FGameplayEffectSpecHandle SpecHandle = WeakThis->MakeOutgoingSpec(StatEffect.EffectClass.Get(), StatEffect.CurrentValue, ContextHandle);
			const FActiveGameplayEffectHandle ActiveHandle = WeakThis->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			EquipmentEntry->GrantedHandles.AddEffectHandle(ActiveHandle);
				});
		}
		
	}
}

void UDHAbilitySystemComponent::RemoveEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry)
{
	for (auto HandleIt = EquipmentEntry->GrantedHandles.ActiveEffects.CreateIterator(); HandleIt; ++HandleIt)
	{
		RemoveActiveGameplayEffect(*HandleIt);
		HandleIt.RemoveCurrent();
	}
}

void UDHAbilitySystemComponent::AddEquipmentAbility(FDoubleHeroesEquipmentEntry* EquipmentEntry)
{
	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	TWeakObjectPtr<UDHAbilitySystemComponent> WeakThis(this);

	if (IsValid(EquipmentEntry->EffectPackage.Ability.AbilityClass.Get()))
	{
		EquipmentEntry->GrantedHandles.GrantedAbility = GrantEquipmentAbility(EquipmentEntry);
	}
	else
	{
		Manager.RequestAsyncLoad(EquipmentEntry->EffectPackage.Ability.AbilityClass.ToSoftObjectPath(),
			[WeakThis, EquipmentEntry]
			{
				EquipmentEntry->GrantedHandles.GrantedAbility = WeakThis->GrantEquipmentAbility(EquipmentEntry);
			});
	}
}

void UDHAbilitySystemComponent::RemoveEquipmentAbility(const FDoubleHeroesEquipmentEntry* EquipmentEntry)
{
	ClearAbility(EquipmentEntry->GrantedHandles.GrantedAbility);
}

void UDHAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	// for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	// {
	// 	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
	// 	GiveAbility(AbilitySpec);
	// }


	//GAS
	for (const TSubclassOf<UGameplayAbility>& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
	
		if (UDoubleHeroesGameplayAbility* DoubleHeroesAbility = Cast<UDoubleHeroesGameplayAbility>(
			AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(DoubleHeroesAbility->InputTag);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();

	//GAS
}

void UDHAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant)
{
	if (PassivesToGrant.Num()>0)
	{
		
	for (const TSubclassOf<UGameplayAbility>& Ability : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
	}
}

void UDHAbilitySystemComponent::InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect)
{
	if(AttributeEffect)
	{
	checkf(AttributeEffect, TEXT("No valid default attributes for this character %s"), *GetAvatarActor()->GetName());
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UDHAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UDoubleHeroesAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
}

void UDHAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FBlueHeroAbilitySet>& InDefaultWeaponAbilities,
                                                         int32 ApplyLevel,  TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FBlueHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if(!AbilitySet.IsValid()) continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UDHAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}

bool UDHAbilitySystemComponent::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayAbilitySpecHandle UDHAbilitySystemComponent::GrantEquipmentAbility(
	const FDoubleHeroesEquipmentEntry* EquipmentEntry)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EquipmentEntry->EffectPackage.Ability.AbilityClass.Get(), EquipmentEntry->EffectPackage.Ability.AbilityLevel);

	if (UDoubleHeroesGameplayAbility* DoubleHeroesAbility = Cast<UDoubleHeroesGameplayAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(DoubleHeroesAbility->InputTag);
	}

	if (UProjectileAbility* ProjectileAbility = Cast<UProjectileAbility>(AbilitySpec.Ability))
	{
		ProjectileAbility->ProjectileToSpawnTag = EquipmentEntry->EffectPackage.Ability.ContextTag;
	}

	return GiveAbility(AbilitySpec);
}

void UDHAbilitySystemComponent::ServerSetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag, int32 AbilityLevel)
{
	SetDynamicProjectile(ProjectileTag, AbilityLevel);
}


void UDHAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                   const FGameplayEffectSpec& EffectSpec,
                                                                   FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	//WidgetController会监听这个
	EffectAssetTags.Broadcast(TagContainer);
}
