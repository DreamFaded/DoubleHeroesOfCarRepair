// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EquipmentManagerComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Items/EquipmentDefinition.h"
#include "Items/EquipmentInstance.h"
#include "Net/UnrealNetwork.h"


UDHAbilitySystemComponent* FDoubleHeroesEquipmentList::GetAbilitySystemComponent()
{
	check(OwnerComponent);
	check(OwnerComponent->GetOwner());
	return Cast<UDHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComponent->GetOwner()));
}

void FDoubleHeroesEquipmentList::AddEquipmentStats(FDoubleHeroesEquipmentEntry* Entry)
{
	if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddEquipmentEffects(Entry);
	}
}

void FDoubleHeroesEquipmentList::RemoveEquipmentStats(FDoubleHeroesEquipmentEntry* Entry)
{
	if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveEquipmentEffects(Entry);
	}
}

void FDoubleHeroesEquipmentList::AddEquipmentAbility(FDoubleHeroesEquipmentEntry* Entry)
{
	if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AddEquipmentAbility(Entry);
	}
}

void FDoubleHeroesEquipmentList::RemoveEquipmentAbility(FDoubleHeroesEquipmentEntry* Entry)
{
	if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->RemoveEquipmentAbility(Entry);
	}
}

UEquipmentInstance* FDoubleHeroesEquipmentList::AddEntry(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage)
{
	check(EquipmentDefinition);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UEquipmentDefinition* EquipmentCDO = GetDefault<UEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<UEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;

	if (!IsValid(InstanceType))
	{
		InstanceType = UEquipmentInstance::StaticClass();
	}
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesEquipmentEntry& Entry = *EntryIt;

		if (Entry.SlotTag.MatchesTagExact(EquipmentCDO->SlotTag))
		{
			RemoveEntry(Entry.Instance);
			break;
		}
	}

	FDoubleHeroesEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EntryTag = EquipmentCDO->ItemTag;
	NewEntry.RarityTag = EquipmentCDO->RarityTag;
	NewEntry.SlotTag = EquipmentCDO->SlotTag;
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.EffectPackage = EffectPackage;
	NewEntry.Instance = NewObject<UEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);

	if (NewEntry.HasStats())
	{
		AddEquipmentStats(&NewEntry);
	}

	if (NewEntry.HasAbility())
	{
		AddEquipmentAbility(&NewEntry);
	}

	NewEntry.Instance->SpawnEquipmentActors(EquipmentCDO->ActorToSpawns);

	MarkItemDirty(NewEntry);
	EquipmentEntryDelegate.Broadcast(NewEntry);

	return NewEntry.Instance;
}

void FDoubleHeroesEquipmentList::RemoveEntry(UEquipmentInstance* EquipmentInstance)
{
	check(OwnerComponent);

	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesEquipmentEntry& Entry = *EntryIt;

		if(Entry.Instance == EquipmentInstance)
		{
			Entry.Instance->DestroySpawnedActors();
			RemoveEquipmentStats(&Entry);
			RemoveEquipmentAbility(&Entry);
			EntryIt.RemoveCurrent();
			MarkArrayDirty(); // 确保移除操作被同步
		}
	}
}

void FDoubleHeroesEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		const FDoubleHeroesEquipmentEntry& Entry = Entries[Index];

		EquipmentEntryDelegate.Broadcast(Entry);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("Equipment Removed: %s"), *Entry.EntryTag.ToString()));
	}
}

void FDoubleHeroesEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		const FDoubleHeroesEquipmentEntry& Entry = Entries[Index];

		EquipmentEntryDelegate.Broadcast(Entry);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(TEXT("Equipment Added: %s"), *Entry.EntryTag.ToString()));
	}
}

void FDoubleHeroesEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		FDoubleHeroesEquipmentEntry& Entry = Entries[Index];

		EquipmentEntryDelegate.Broadcast(Entry);
	}
}


UEquipmentManagerComponent::UEquipmentManagerComponent() : EquipmentList(this)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEquipmentManagerComponent, EquipmentList);
}

void UEquipmentManagerComponent::EquipItem(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerEquipItem(EquipmentDefinition, EffectPackage);
		return;
	}
	
	if(UEquipmentInstance* Result = EquipmentList.AddEntry(EquipmentDefinition, EffectPackage))
	{
		Result->OnEquipped();
	}
}

void UEquipmentManagerComponent::UnEquipItem(UEquipmentInstance* EquipmentInstance)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerUnEquipItem(EquipmentInstance);
		return;
	}

	EquipmentInstance->OnUnequipped();
	EquipmentList.RemoveEntry(EquipmentInstance);
}

void UEquipmentManagerComponent::ServerEquipItem_Implementation(
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage)
{
	EquipItem(EquipmentDefinition, EffectPackage);
}

void UEquipmentManagerComponent::ServerUnEquipItem_Implementation(UEquipmentInstance* EquipmentInstance)
{
	UnEquipItem(EquipmentInstance);
}



