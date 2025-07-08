// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Items/EquipmentTypes.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InventoryComponent.generated.h"


class UInventoryComponent;
class UEquipmentStatEffects;
class UItemTypesToTables;
class UEquipmentDefinition;
struct FDoubleHeroesInventoryList;
struct FMasterItemDefinition;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_TwoParams(FEquipmentItemUsed, const TSubclassOf<UEquipmentDefinition>& /* Equipment Definition */, const FEquipmentEffectPackage&)

/*USTRUCT()
struct FPackagedInventory
{
	GENERATED_BODY()

	virtual ~FPackagedInventory() = default;

	UPROPERTY()
	TArray<FGameplayTag> ItemTags;

	UPROPERTY()
	TArray<int32> ItemQuantities;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FPackagedInventory> : TStructOpsTypeTraitsBase2<FPackagedInventory>
{
	enum
	{
		WithNetSerializer = true
	};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryPackageSignature, const FPackagedInventory&);

*/

USTRUCT(BlueprintType)
struct FDoubleHeroesInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FText ItemName = FText();

	UPROPERTY(BlueprintReadOnly)
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadOnly)
	int64 ItemID = 0;

	UPROPERTY(BlueprintReadOnly)
	FEquipmentEffectPackage EffectPackage = FEquipmentEffectPackage();

	bool IsValid() const
	{
		return ItemID != 0;
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDirtyInventoryItemSignature, const FDoubleHeroesInventoryEntry& /* Dirty Item */);
DECLARE_MULTICAST_DELEGATE_OneParam(FInventoryItemRemovedSignature, const int64 /* ItemID */);

USTRUCT()
struct FDoubleHeroesInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FDoubleHeroesInventoryList() : OwnerComponent(nullptr)
	{}

	FDoubleHeroesInventoryList(UInventoryComponent* InComponent)
		: OwnerComponent(InComponent)
	{}

	void AddItem(const FGameplayTag& ItemTag, int32 NumItems = 1);
	void AddOneItem(const FGameplayTag& ItemTag);
	
	void AddUnEquippedItem(const FGameplayTag& ItemTag, const FEquipmentEffectPackage& EffectPackage);
	void RemoveItem(const FDoubleHeroesInventoryEntry& InventoryEntry, int32 NumItems = 1);
	bool HasEnough(const FGameplayTag& ItemTag, int32 NumItems = 1);
	uint64 GenerateID();
	void SetStats(UEquipmentStatEffects* InStats);
	void RollForStats(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, FDoubleHeroesInventoryEntry* Entry);

	// FFastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);

	

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FDoubleHeroesInventoryEntry, FDoubleHeroesInventoryList>(Entries, DeltaParms, *this);
	}

	FDirtyInventoryItemSignature DirtyItemDelegate;
	FInventoryItemRemovedSignature InventoryItemRemovedDelegate;

private:

	friend UInventoryComponent;

	UPROPERTY()
	TArray<FDoubleHeroesInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UInventoryComponent> OwnerComponent;

	UPROPERTY(NotReplicated)
	int64 LastAssignedID = 0;

	UPROPERTY(NotReplicated)
	TWeakObjectPtr<UEquipmentStatEffects> WeakStats;

	
};

template<>
struct TStructOpsTypeTraits<FDoubleHeroesInventoryList> : TStructOpsTypeTraitsBase2<FDoubleHeroesInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// FInventoryPackagedSignature InventoryPackagedSignature;

	FEquipmentItemUsed EquipmentItemDelegate;

	UPROPERTY(Replicated)
	FDoubleHeroesInventoryList InventoryList;
	
	// Sets default values for this component's properties
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItem(const FGameplayTag& ItemTag, int32 NumItems = 1);

	//添加一个Item
	void AddOneItem(const FGameplayTag& ItemTag);

	UFUNCTION(BlueprintCallable)
	void UseItem(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems);

	UFUNCTION(BlueprintPure)
	FMasterItemDefinition GetItemDefinitionByTag(const FGameplayTag& ItemTag) const;

	TArray<FDoubleHeroesInventoryEntry> GetInventoryEntries();

	void AddUnEquippedItemEntry(const FGameplayTag& ItemTag, const FEquipmentEffectPackage& EffectPackage);
	
private:

	friend  FDoubleHeroesInventoryList;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TMap<FGameplayTag, int32> InventoryTagMap;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Stat Effects")
	TObjectPtr<UEquipmentStatEffects> StatEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values|Item Definitions")
	TObjectPtr<UItemTypesToTables> InventoryDefinitions;

	UFUNCTION(Server, Reliable)
	void ServerAddItem(const FGameplayTag& ItemTag, int32 NumItems);

	UFUNCTION(Server, Reliable)
	void ServerAddOneItem(const FGameplayTag& ItemTag);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems);

	bool ServerUseItem_Validate(const FDoubleHeroesInventoryEntry& Entry, int32 NumItems);


	/*UPROPERTY(ReplicatedUsing=OnRep_CachedInventory)
	FPackagedInventory CachedInventory;
	

	void PackageInventory(FPackagedInventory& OutInventory);

	void ReconstructInventoryMap(const FPackagedInventory& Inventory);

	UFUNCTION()
	void OnRep_CachedInventory();*/
};
