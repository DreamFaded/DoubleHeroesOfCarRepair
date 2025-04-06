// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DoubleHeroesItemManagerComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "Items/DoubleHeroesItemDefinition.h"
#include "Items/DoubleHeroesItemInstance.h"
#include "Net/UnrealNetwork.h"



//////////////////////////////////////////////////////////////////////
// FDoubleHeroesAppliedItemEntry

FString FDoubleHeroesAppliedItemEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(ItemDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FDoubleHeroesItemList

void FDoubleHeroesItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FDoubleHeroesAppliedItemEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FDoubleHeroesItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FDoubleHeroesAppliedItemEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FDoubleHeroesItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

UDHAbilitySystemComponent* FDoubleHeroesItemList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UDHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UDoubleHeroesItemInstance* FDoubleHeroesItemList::AddEntry(TSubclassOf<UDoubleHeroesItemDefinition> ItemDefinition)
{
	UDoubleHeroesItemInstance* Result = nullptr;

	check(ItemDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UDoubleHeroesItemDefinition* ItemCDO = GetDefault<UDoubleHeroesItemDefinition>(ItemDefinition);

	TSubclassOf<UDoubleHeroesItemInstance> InstanceType = ItemCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UDoubleHeroesItemInstance::StaticClass();
	}
	
	FDoubleHeroesAppliedItemEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemDefinition = ItemDefinition;
	NewEntry.Instance = NewObject<UDoubleHeroesItemInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const UDoubleHeroesAbilitySet> AbilitySet : ItemCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnItemActors(ItemCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FDoubleHeroesItemList::RemoveEntry(UDoubleHeroesItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesAppliedItemEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UDHAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyItemActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UDoubleHeroesItemManagerComponent

UDoubleHeroesItemManagerComponent::UDoubleHeroesItemManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, ItemList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UDoubleHeroesItemManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemList);
}

UDoubleHeroesItemInstance* UDoubleHeroesItemManagerComponent::EquipItem(TSubclassOf<UDoubleHeroesItemDefinition> ItemClass)
{
	UDoubleHeroesItemInstance* Result = nullptr;
	if (ItemClass != nullptr)
	{
		Result = ItemList.AddEntry(ItemClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UDoubleHeroesItemManagerComponent::UnequipItem(UDoubleHeroesItemInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		ItemList.RemoveEntry(ItemInstance);
	}
}

bool UDoubleHeroesItemManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FDoubleHeroesAppliedItemEntry& Entry : ItemList.Entries)
	{
		UDoubleHeroesItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UDoubleHeroesItemManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDoubleHeroesItemManagerComponent::UninitializeComponent()
{
	TArray<UDoubleHeroesItemInstance*> AllItemInstances;

	// gathering all instances before removal to avoid side effects affecting the Item list iterator	
	for (const FDoubleHeroesAppliedItemEntry& Entry : ItemList.Entries)
	{
		AllItemInstances.Add(Entry.Instance);
	}

	for (UDoubleHeroesItemInstance* EquipInstance : AllItemInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UDoubleHeroesItemManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing DoubleHeroesItemInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FDoubleHeroesAppliedItemEntry& Entry : ItemList.Entries)
		{
			UDoubleHeroesItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UDoubleHeroesItemInstance* UDoubleHeroesItemManagerComponent::GetFirstInstanceOfType(TSubclassOf<UDoubleHeroesItemInstance> InstanceType)
{
	for (FDoubleHeroesAppliedItemEntry& Entry : ItemList.Entries)
	{
		if (UDoubleHeroesItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UDoubleHeroesItemInstance*> UDoubleHeroesItemManagerComponent::GetItemInstancesOfType(TSubclassOf<UDoubleHeroesItemInstance> InstanceType) const
{
	TArray<UDoubleHeroesItemInstance*> Results;
	for (const FDoubleHeroesAppliedItemEntry& Entry : ItemList.Entries)
	{
		if (UDoubleHeroesItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

