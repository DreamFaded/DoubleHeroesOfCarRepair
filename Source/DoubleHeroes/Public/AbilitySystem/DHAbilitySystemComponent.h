// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DHAbilitySystemComponent.generated.h"

struct FDoubleHeroesEquipmentEntry;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	FEffectAssetTags EffectAssetTags;

	// void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void SetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel = 1);
	
	void AddEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void RemoveEquipmentEffects(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void AddEquipmentAbility(FDoubleHeroesEquipmentEntry* EquipmentEntry);
	void RemoveEquipmentAbility(const FDoubleHeroesEquipmentEntry* EquipmentEntry);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	void InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect);

protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability", meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FBlueHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Ability")
	void RemovedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

private:
	FGameplayAbilitySpecHandle ActiveProjectileAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Projectile Ability")
	TSubclassOf<UGameplayAbility> DynamicProjectileAbility;

	FGameplayAbilitySpecHandle GrantEquipmentAbility(const FDoubleHeroesEquipmentEntry* EquipmentEntry);

	UFUNCTION(Server, Reliable)
	void ServerSetDynamicProjectile(const FGameplayTag& ProjectileTag, int32 AbilityLevel);
};
