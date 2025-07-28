// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnCombatComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Character/BlueHeroCharacter.h"
#include "Character/DoubleHeroesBaseCharacter.h"
#include "Data/CharacterClassInfo.h"
#include "DoubleHeroesComponent/DoubleHeroesDebugHelper.h"
#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/ItemWeapon.h"
#include "Items/Weapons/DoubleHeroesWeaponBase.h"
#include "Libraries/DHAbilitySystemLibrary.h"
#include "Net/UnrealNetwork.h"
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
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	EquippedItem->AttachToComponent(Character->GetMesh(), TransformRules, TEXT("RightHandSocket"));
	// const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	// if (HandSocket)
	// {
		// ItemToPickup->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, HandSocket->SocketName);
		// HandSocket->AttachActor(EquippedItem, Character->GetMesh());
	// }
	
	EquippedItem->SetOwner(Character);
	
	// 获取玩家控制器
	if (APlayerController* PC = Cast<APlayerController>(Character->GetController()))
	{
		// 获取增强输入本地玩家子系统
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (FDoubleHeroesItemData* ItemData = ItemToPickup->TagItemDataMap.Find(Character->CharacterTag))
			{
				Character->GetMesh()->LinkAnimClassLayers(ItemData->ItemAnimLayerToLink);
				Subsystem->AddMappingContext(ItemData->ItemInputMappingContext, 1);
			}
			
		}
	}
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

	DOREPLIFETIME(UPawnCombatComponent, EquippedItem);
}
