// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DoubleHeroesAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "DoubleHeroesItemManagerComponent.generated.h"

class UActorComponent;
class UDHAbilitySystemComponent;
class UDoubleHeroesItemDefinition;
class UDoubleHeroesItemInstance;
class UDoubleHeroesItemManagerComponent;
class UObject;
struct FFrame;
struct FDoubleHeroesItemList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied Item */
USTRUCT(BlueprintType)
struct FDoubleHeroesAppliedItemEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FDoubleHeroesAppliedItemEntry()
	{}

	FString GetDebugString() const;

private:
	friend FDoubleHeroesItemList;
	friend UDoubleHeroesItemManagerComponent;

	// The Item class that got equipped
	UPROPERTY()
	TSubclassOf<UDoubleHeroesItemDefinition> ItemDefinition;

	UPROPERTY()
	TObjectPtr<UDoubleHeroesItemInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FDoubleHeroesAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied Item */
USTRUCT(BlueprintType)
struct FDoubleHeroesItemList : public FFastArraySerializer
{
	GENERATED_BODY()

	FDoubleHeroesItemList()
		: OwnerComponent(nullptr)
	{
	}

	FDoubleHeroesItemList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FDoubleHeroesAppliedItemEntry, FDoubleHeroesItemList>(Entries, DeltaParms, *this);
	}

	UDoubleHeroesItemInstance* AddEntry(TSubclassOf<UDoubleHeroesItemDefinition> ItemDefinition);
	void RemoveEntry(UDoubleHeroesItemInstance* Instance);

private:
	UDHAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UDoubleHeroesItemManagerComponent;

private:
	// Replicated list of Item entries
	UPROPERTY()
	TArray<FDoubleHeroesAppliedItemEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FDoubleHeroesItemList> : public TStructOpsTypeTraitsBase2<FDoubleHeroesItemList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages Item applied to a pawn
 */
UCLASS(BlueprintType, Const)
class UDoubleHeroesItemManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UDoubleHeroesItemManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UDoubleHeroesItemInstance* EquipItem(TSubclassOf<UDoubleHeroesItemDefinition> ItemDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UDoubleHeroesItemInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDoubleHeroesItemInstance* GetFirstInstanceOfType(TSubclassOf<UDoubleHeroesItemInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UDoubleHeroesItemInstance*> GetItemInstancesOfType(TSubclassOf<UDoubleHeroesItemInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FDoubleHeroesItemList ItemList;
};
