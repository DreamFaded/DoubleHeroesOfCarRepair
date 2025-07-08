// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Items/EquipmentDefinition.h"
#include "Items/EquipmentStatEffects.h"
#include "Items/EquipmentTypes.h"
#include "Items/ItemTypes.h"
#include "Items/ItemTypesToTables.h"
#include "Libraries/DHAbilitySystemLibrary.h"
#include "Net/UnrealNetwork.h"


namespace DoubleHeroesGameplayTags::Static
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Category_Equipment, "Item.Equipment");
}



void FDoubleHeroesInventoryList::AddItem(const FGameplayTag& ItemTag, int32 NumItems)
{
	// if (ItemTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Item.Equipment"))))
	if (ItemTag.MatchesTag(DoubleHeroesGameplayTags::Static::Category_Equipment))
	{
		goto MakeNew;
	}
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemTag.MatchesTagExact(ItemTag))
		{
			Entry.Quantity += NumItems;

			MarkItemDirty(Entry);

			if (OwnerComponent->GetOwner()->HasAuthority())
			{
				DirtyItemDelegate.Broadcast(Entry);
			}
			return;
		}
	}

	MakeNew:
	FMasterItemDefinition Item = OwnerComponent->GetItemDefinitionByTag(ItemTag);

	FDoubleHeroesInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemTag = ItemTag;
	NewEntry.ItemName = Item.ItemName;
	NewEntry.Quantity = NumItems;
	NewEntry.ItemID = GenerateID();

	if (NewEntry.ItemTag.MatchesTag(DoubleHeroesGameplayTags::Static::Category_Equipment) && IsValid(WeakStats.Get()))
	{
		RollForStats(Item.EquipmentItemProps.EquipmentClass, &NewEntry);
	}
	
	if (OwnerComponent->GetOwner()->HasAuthority())
	{
		//InventoryWidgetController进行UI更新
		DirtyItemDelegate.Broadcast(NewEntry);
	}

	MarkItemDirty(NewEntry);
}

void FDoubleHeroesInventoryList::AddOneItem(const FGameplayTag& ItemTag)
{
	if (ItemTag.MatchesTag(DoubleHeroesGameplayTags::Static::Category_Equipment))
	{
		goto MakeNew;
	}
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemTag.MatchesTagExact(ItemTag))
		{
			MarkItemDirty(Entry);

			if (OwnerComponent->GetOwner()->HasAuthority())
			{
				DirtyItemDelegate.Broadcast(Entry);
			}
			return;
		}
	}

	MakeNew:
	FMasterItemDefinition Item = OwnerComponent->GetItemDefinitionByTag(ItemTag);

	FDoubleHeroesInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemTag = ItemTag;
	NewEntry.ItemName = Item.ItemName;
	NewEntry.ItemID = GenerateID();

	if (NewEntry.ItemTag.MatchesTag(DoubleHeroesGameplayTags::Static::Category_Equipment) && IsValid(WeakStats.Get()))
	{
		RollForStats(Item.EquipmentItemProps.EquipmentClass, &NewEntry);
	}
	
	if (OwnerComponent->GetOwner()->HasAuthority())
	{
		DirtyItemDelegate.Broadcast(NewEntry);
	}

	MarkItemDirty(NewEntry);
}


void FDoubleHeroesInventoryList::AddUnEquippedItem(const FGameplayTag& ItemTag,
                                                   const FEquipmentEffectPackage& EffectPackage)
{
	const FMasterItemDefinition Item = OwnerComponent->GetItemDefinitionByTag(ItemTag);

	FDoubleHeroesInventoryEntry &NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.ItemTag = ItemTag;
	NewEntry.ItemName = Item.ItemName;
	NewEntry.Quantity = 1;
	NewEntry.ItemID = GenerateID();
	NewEntry.EffectPackage = EffectPackage;

	DirtyItemDelegate.Broadcast(NewEntry);
	MarkItemDirty(NewEntry);
}

void FDoubleHeroesInventoryList::RemoveItem(const FDoubleHeroesInventoryEntry& InventoryEntry, int32 NumItems)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemID == InventoryEntry.ItemID)
		{
			Entry.Quantity -= NumItems;

			if (Entry.Quantity > 0)
			{
				MarkItemDirty(Entry);
				
				if (OwnerComponent->GetOwner()->HasAuthority())
				{
					DirtyItemDelegate.Broadcast(Entry);
				}
			}
			else
			{
				InventoryItemRemovedDelegate.Broadcast(Entry.ItemID);
				EntryIt.RemoveCurrent();
				MarkArrayDirty();
			}

			break;
		}
	}
}

bool FDoubleHeroesInventoryList::HasEnough(const FGameplayTag& ItemTag, int32 NumItems)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FDoubleHeroesInventoryEntry& Entry = *EntryIt;

		if (Entry.ItemTag.MatchesTagExact(ItemTag))
		{
			if(Entry.Quantity >= NumItems)
			{
				return true;
			}
		}
	}
	
	return false;
}

uint64 FDoubleHeroesInventoryList::GenerateID()
{
	int64 NewID = ++LastAssignedID;

	int32 SignatureIndex = 0;
	while (SignatureIndex < 12)
	{
		if (FMath::RandRange(0, 100) < 85)
		{
			NewID |= (uint64)1 << FMath::RandRange(0, 63);
		}
		++SignatureIndex;
	}
	return NewID;
}

void FDoubleHeroesInventoryList::SetStats(UEquipmentStatEffects* InStats)
{
	WeakStats = InStats;
}



void FDoubleHeroesInventoryList::RollForStats(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition,
	FDoubleHeroesInventoryEntry* Entry)
{
	UEquipmentStatEffects* StatEffects = WeakStats.Get();
	const UEquipmentDefinition* EquipmentCDO = GetDefault<UEquipmentDefinition>(EquipmentDefinition);

	if (EquipmentCDO->bForceAbilityRoll || FMath::FRandRange(0.f, 1.f) < EquipmentCDO->ProbabilityToRollAbility)
	{
		const int32 RandomIndex = FMath::RandRange(0, EquipmentCDO->PossibleAbilityRolls.Num() - 1);
		const FGameplayTag& RandomTag = EquipmentCDO->PossibleAbilityRolls.GetByIndex(RandomIndex);
	
		for (const auto& Pair : StatEffects->MasterStatMap)
		{
			if (RandomTag.MatchesTag(Pair.Key))
			{
				if (const FEquipmentAbilityGroup* PossibleAbility = UDHAbilitySystemLibrary::GetDataTableRowByTag<
					FEquipmentAbilityGroup>(Pair.Value, RandomTag))
				{
					if (FMath::FRandRange(0.f, 1.f) < PossibleAbility->ProbabilityToSelect)
					{
						Entry->EffectPackage.Ability = *PossibleAbility;
						break;
					}
				}
			}
		}
	}

	const int32 NewStatsToRoll = FMath::RandRange(EquipmentCDO->MinPossibleStats, EquipmentCDO->MaxPossibleStats);
	int32 StatRollIndex = 0;
	while (StatRollIndex < NewStatsToRoll)
	{
		const int32 RandomIndex = FMath::RandRange(0, EquipmentCDO->PossibleStatsRolls.Num() - 1);
		const FGameplayTag& RandomTag = EquipmentCDO->PossibleStatsRolls.GetByIndex(RandomIndex);

		for (const auto& Pair : StatEffects->MasterStatMap)
		{
			if (RandomTag.MatchesTag(Pair.Key))
			{
				if (const FEquipmentStatEffectGroup* PossibleStat =
					UDHAbilitySystemLibrary::GetDataTableRowByTag<FEquipmentStatEffectGroup>(
						Pair.Value, RandomTag))
				{
					if (FMath::FRandRange(0.f, 1.f) < PossibleStat->ProbabilityToSelect)
					{
						FEquipmentStatEffectGroup NewStat = *PossibleStat;

						NewStat.CurrentValue = PossibleStat->bFractionStat
							                        ? FMath::FRandRange(PossibleStat->MinStatLevel,
							                                            PossibleStat->MaxStatLevel)
							                        : FMath::TruncToInt(FMath::FRandRange(
								                        PossibleStat->MinStatLevel, PossibleStat->MaxStatLevel));
						Entry->EffectPackage.StatEffects.Add(NewStat);
						++StatRollIndex;
						break;
					}
				}
			}
		}
	}
	
}

void FDoubleHeroesInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	// If you can figure out what to do with this go for it. I don't know what it is reliably good for.

	for (const int32 Index : RemovedIndices)
	{
		const FDoubleHeroesInventoryEntry& Entry = Entries[Index];

		InventoryItemRemovedDelegate.Broadcast(Entry.ItemID);
	}
}

void FDoubleHeroesInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Index : ChangedIndices)
	{
		FDoubleHeroesInventoryEntry& Entry = Entries[Index];

		DirtyItemDelegate.Broadcast(Entry);
	}
}

void FDoubleHeroesInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		FDoubleHeroesInventoryEntry& Entry = Entries[Index];

		DirtyItemDelegate.Broadcast(Entry);
	}
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent() : InventoryList(this)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
	// DOREPLIFETIME(UInventoryComponent, CachedInventory);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		InventoryList.SetStats(StatEffects);
	}
}


void UInventoryComponent::AddItem(const FGameplayTag& ItemTag, int32 NumItems)
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerAddItem(ItemTag, NumItems);
		return;
	}

	InventoryList.AddItem(ItemTag, NumItems);
	
}

void UInventoryComponent::AddOneItem(const FGameplayTag& ItemTag)
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerAddOneItem(ItemTag);
		return;
	}
	InventoryList.AddOneItem(ItemTag);
}

void UInventoryComponent::UseItem(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems)
{
	AActor* Owner = GetOwner();
	if(!IsValid(Owner)) return;

	if (!Owner->HasAuthority())
	{
		ServerUseItem(Entry, NumItems);
		return;
	}

	if (InventoryList.HasEnough(Entry.ItemTag, NumItems))
	{
		const FMasterItemDefinition Item = GetItemDefinitionByTag(Entry.ItemTag);

		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
		{
			if (IsValid(Item.ConsumableProps.ItemEffectClass))
			{
				const FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();

				const FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(Item.ConsumableProps.ItemEffectClass,
					Item.ConsumableProps.ItemEffectLevel, ContextHandle);
				OwnerASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				InventoryList.RemoveItem(Entry);

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta,
					FString::Printf(TEXT("Server Item Used %s"), *Item.ItemTag.ToString()));
			}
			if (IsValid(Item.EquipmentItemProps.EquipmentClass))
			{
				EquipmentItemDelegate.Broadcast(Item.EquipmentItemProps.EquipmentClass, Entry.EffectPackage);
				InventoryList.RemoveItem(Entry);
			}
		}
	}

	
}

FMasterItemDefinition UInventoryComponent::GetItemDefinitionByTag(const FGameplayTag& ItemTag) const
{
	checkf(InventoryDefinitions, TEXT("No Inventory Definitions Inside Component %s"), *GetNameSafe(this));

	for(const auto& Pair : InventoryDefinitions->TagsToTables)
	{
		if (ItemTag.MatchesTag(Pair.Key))
		{
			if(const FMasterItemDefinition* ValidItem = UDHAbilitySystemLibrary::GetDataTableRowByTag<FMasterItemDefinition>(Pair.Value, ItemTag))
			{
				return *ValidItem;
			}
		}
	}
	return FMasterItemDefinition();
}

TArray<FDoubleHeroesInventoryEntry> UInventoryComponent::GetInventoryEntries()
{
	return InventoryList.Entries;
}

void UInventoryComponent::AddUnEquippedItemEntry(const FGameplayTag& ItemTag,
	const FEquipmentEffectPackage& EffectPackage)
{
	InventoryList.AddUnEquippedItem(ItemTag, EffectPackage);
}

void UInventoryComponent::ServerAddOneItem_Implementation(const FGameplayTag& ItemTag)
{
	AddOneItem(ItemTag);
}

bool UInventoryComponent::ServerUseItem_Validate(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems)
{
	return Entry.IsValid() && InventoryList.HasEnough(Entry.ItemTag, NumItems);
}

/*
*bool FPackagedInventory::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	SafeNetSerializeTArray_WithNetSerialize<100>(Ar, ItemTags, Map);
	SafeNetSerializeTArray_Default<100>(Ar, ItemQuantities);

	bOutSuccess = true;
	return true;
}
void UInventoryComponent::PackageInventory(FPackagedInventory& OutInventory)
{
	OutInventory.ItemTags.Empty();
	OutInventory.ItemQuantities.Empty();

	for (const auto& Pair : InventoryTagMap)
	{
		if (Pair.Value > 0)
		{
			OutInventory.ItemTags.Add(Pair.Key);
			OutInventory.ItemQuantities.Add(Pair.Value);
		}
	}
}

void UInventoryComponent::ReconstructInventoryMap(const FPackagedInventory& Inventory)
{
	InventoryTagMap.Empty();
	
	for (int32 i = 0; i < Inventory.ItemTags.Num(); ++i)
	{
		InventoryTagMap.Emplace(Inventory.ItemTags[i], Inventory.ItemQuantities[i]);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta,
			FString::Printf(TEXT("Tag Added %s // Quantity Added: %d"), *Inventory.ItemTags[i].ToString(),
				Inventory.ItemQuantities[i]));
	}
}

void UInventoryComponent::OnRep_CachedInventory()
{
	ReconstructInventoryMap(CachedInventory);
}*/

void UInventoryComponent::ServerAddItem_Implementation(const FGameplayTag& ItemTag, int32 NumItems)
{
	AddItem(ItemTag, NumItems);
}

void UInventoryComponent::ServerUseItem_Implementation(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems)
{
	if(InventoryList.HasEnough(Entry.ItemTag, NumItems))
	{
		UseItem(Entry, NumItems);
	}
}



