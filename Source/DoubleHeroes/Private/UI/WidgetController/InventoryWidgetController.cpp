﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/InventoryWidgetController.h"

#include "Components/InventoryComponent.h"
#include "Interface/InventoryInterface.h"
#include "Items/ItemTypes.h"

void UInventoryWidgetController::SetOwningActor(AActor* InOwner)
{
	OwningActor = InOwner;
}

void UInventoryWidgetController::BindCallbacksToDependencies()
{
	OwningInventory = IInventoryInterface::Execute_GetInventoryComponent(OwningActor);

	if (IsValid(OwningInventory))
	{
		OwningInventory->InventoryList.DirtyItemDelegate.AddLambda(
			[this](const FDoubleHeroesInventoryEntry& DirtyItem)
		{
				// FMasterItemDefinition Item = OwningInventory->GetItemDefinitionByTag(DirtyItem.ItemTag);
				// Item.ItemQuantity = DirtyItem.Quantity;

				InventoryEntryDelegate.Broadcast(DirtyItem);
		});

		OwningInventory->InventoryList.InventoryItemRemovedDelegate.AddLambda(
			[this](const int64 ItemID)
		{
				OnInventoryItemRemoved.Broadcast(ItemID);
		});
	}
}

void UInventoryWidgetController::BroadcastInitialValues()
{
	if (IsValid(OwningInventory))
	{
		for (const FDoubleHeroesInventoryEntry& Entry : OwningInventory->GetInventoryEntries())
		{
			// FMasterItemDefinition Item = OwningInventory->GetItemDefinitionByTag(Entry.ItemTag);
			// Item.ItemQuantity = Entry.Quantity;

			InventoryEntryDelegate.Broadcast(Entry);
		}
	}
}

void UInventoryWidgetController::BroadcastInventoryContents()
{
	// if (IsValid(OwningInventory))
	// {
	// 	TMap<FGameplayTag, int32> LocalInventoryMap = OwningInventory->GetInventoryTagMap();
	//
	// 	ScrollBoxResetDelegate.Broadcast();
	//
	// 	for (const auto& Pair : LocalInventoryMap)
	// 	{
	// 		FMasterItemDefinition Item = OwningInventory->GetItemDefinitionByTag(Pair.Key);
	// 		Item.ItemQuantity = Pair.Value;
	// 		InventoryItemDelegate.Broadcast(Item);
	// 	}
	//
	// 	InventoryBroadcastComplete.Broadcast();
	// }
}
