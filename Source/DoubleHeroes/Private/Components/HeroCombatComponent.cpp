// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HeroCombatComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Items/Weapons/DoubleHeroesWeapon.h"

ADoubleHeroesWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ADoubleHeroesWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::PickupItem(AItemBase* ItemToPickup)
{
	Super::PickupItem(ItemToPickup);
	ApplyInputMappingForItem(ItemToPickup);
}

void UHeroCombatComponent::ApplyInputMappingForItem(AItemBase* Item)
{
	if (!Item || !Character) return;
	// Character->GetMesh()->LinkAnimClassLayers(DoubleHeroesItemData.ItemAnimLayerToLink);
	
}

