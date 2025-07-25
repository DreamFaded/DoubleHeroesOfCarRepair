// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnCombatComponent.h"

#include "Character/DoubleHeroesBaseCharacter.h"
#include "DoubleHeroesComponent/DoubleHeroesDebugHelper.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/DoubleHeroesWeaponBase.h"
#include "Weapon/Weapon.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
                                                 ADoubleHeroesWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	check(InWeaponToRegister);
	if(CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon with tag %s is already registered. Skipping registration."), *InWeaponTagToRegister.ToString());
		return;
	}

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
	const FString WeaponString = FString::Printf(TEXT("A weapon named : %s has been registered using the tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	Debug::Print(WeaponString);
}

ADoubleHeroesWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (ADoubleHeroesWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

ADoubleHeroesWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(Character == nullptr || WeaponToEquip == nullptr) return;
	
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
}

void UPawnCombatComponent::PickupItem(AItemBase* ItemToPickup)
{
	if(Character == nullptr || ItemToPickup == nullptr) return;
	
	EquippedItem = ItemToPickup;
	EquippedItem->SetItemState(EItemState::EIS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		EquippedItem->AttachToComponent(Character->GetMesh(), TransformRules, FName("RightHandSocket"));
		// HandSocket->AttachActor(EquippedItem, Character->GetMesh());
	}
	// 验证是否正确附加
	if (EquippedItem->GetAttachParentSocketName() == FName("RightHandSocket"))
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to attach item"));
	}
	EquippedItem->SetOwner(Character);
}

void UPawnCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void UPawnCombatComponent::OnRep_EquippedItem()
{
}

void UPawnCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
