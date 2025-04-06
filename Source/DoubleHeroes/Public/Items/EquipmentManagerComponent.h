// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "EquipmentManagerComponent.generated.h"

class UEquipmentInstance;
class UEquipmentDefinition;
class UDHAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FDoubleHeroesEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag EntryTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag SlotTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag RarityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FEquipmentEffectPackage EffectPackage = FEquipmentEffectPackage();

	UPROPERTY(NotReplicated)
	FEquipmentGrantedHandles GrantedHandles = FEquipmentGrantedHandles();

	bool HasStats() const
	{
		return !EffectPackage.StatEffects.IsEmpty();
	}

	bool HasAbility() const
	{
		return EffectPackage.Ability.AbilityTag.IsValid();
	}

private:

	friend UEquipmentManagerComponent;
	friend struct FDoubleHeroesEquipmentList;

	UPROPERTY()
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> Instance = nullptr;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEquipmentEntrySignature, const FDoubleHeroesEquipmentEntry& /* Equipment Entry */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnEquippedEntry, const FDoubleHeroesEquipmentEntry& /* Equipment Entry */)

USTRUCT()
struct FDoubleHeroesEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FDoubleHeroesEquipmentList() : OwnerComponent(nullptr){}

	FDoubleHeroesEquipmentList(UActorComponent* InComponent)
		: OwnerComponent(InComponent)
	{}

	UDHAbilitySystemComponent* GetAbilitySystemComponent();
	void AddEquipmentStats(FDoubleHeroesEquipmentEntry* Entry);
	void RemoveEquipmentStats(FDoubleHeroesEquipmentEntry* Entry);
	void AddEquipmentAbility(FDoubleHeroesEquipmentEntry* Entry);
	void RemoveEquipmentAbility(FDoubleHeroesEquipmentEntry* Entry);
	UEquipmentInstance* AddEntry(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage);
	void RemoveEntry(UEquipmentInstance* Instance);

	// FastArraySerializer Contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FDoubleHeroesEquipmentEntry, FDoubleHeroesEquipmentList>(Entries, DeltaParms, *this);
	}

	FEquipmentEntrySignature EquipmentEntryDelegate;
	FOnUnEquippedEntry UnEquippedEntryDelegate;

private:

	UPROPERTY()
	TArray<FDoubleHeroesEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
	
};

template<>
struct TStructOpsTypeTraits<FDoubleHeroesEquipmentList> : TStructOpsTypeTraitsBase2<FDoubleHeroesEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(Replicated)
	FDoubleHeroesEquipmentList EquipmentList;
	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void EquipItem(const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage);
	void UnEquipItem(UEquipmentInstance* EquipmentInstance);

private:

	UFUNCTION(Server, Reliable)
	void ServerEquipItem(TSubclassOf<UEquipmentDefinition> EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage);

	UFUNCTION(Server, Reliable)
	void ServerUnEquipItem(UEquipmentInstance* EquipmentInstance);
};
