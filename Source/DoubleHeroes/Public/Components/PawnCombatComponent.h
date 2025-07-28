// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/BlueHeroCharacter.h"
#include "PawnExtensionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class ABlueHeroCharacter;
class ADoubleHeroesBaseCharacter;
class AItemWeapon;
class ADoubleHeroesWeaponBase;
struct FGameplayTag;
class AWeapon;
class AItemBase;

UCLASS()
class DOUBLEHEROES_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ADoubleHeroesWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	ADoubleHeroesWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "DoubleHeroes|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "DoubleHeroes|Combat")
	ADoubleHeroesWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(Category = "DoubleHeroes|Combat")
	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION(Category = "DoubleHeroes|Combat")
	virtual void PickupItem(AItemBase* ItemToPickup);

	UPROPERTY()
	ADoubleHeroesBaseCharacter* Character;

	UPROPERTY()
	ABlueHeroCharacter* BlueHeroCharacter;
	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedItem)
	AItemBase* EquippedItem;

	UPROPERTY()
	AItemWeapon* Equippeda;

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_EquippedItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	TMap<FGameplayTag, ADoubleHeroesWeaponBase*> CharacterCarriedWeaponMap;

	
};
