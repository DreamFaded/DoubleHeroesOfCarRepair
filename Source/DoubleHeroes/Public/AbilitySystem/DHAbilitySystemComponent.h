// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DHAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
struct FDoubleHeroesEquipmentEntry;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	bool bStartupAbilitiesGiven = false;
	FActivatePassiveEffect ActivatePassiveEffect;

	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	

	// void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel = 1);
	
	void AddEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void RemoveEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void AddEquipmentAbility(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void RemoveEquipmentAbility(const FDoubleHeroesEquipmentEntry* EquipmentEntry);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect);

	void UpdateAbilityStatuses(int32 Level);

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FBlueHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability")
	void RemovedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

private:
	FGameplayAbilitySpecHandle ActiveProjectileAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Projectile Ability")
	TSubclassOf<UGameplayAbility> DynamicProjectileAbility;

	FGameplayAbilitySpecHandle GrantEquipmentAbility(const FDoubleHeroesEquipmentEntry* EquipmentEntry);

	UFUNCTION(Server, Reliable)
	void ServerSetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel);
};
