// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetController.h"
#include "InventoryWidgetController.generated.h"

struct FMasterItemDefinition;
class UInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemSignature, const FDoubleHeroesInventoryEntry&, Entry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRemoved, const int64, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryBroadcastComplete);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScrollBoxResetSignature);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class DOUBLEHEROES_API UInventoryWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FInventoryItemSignature InventoryEntryDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemRemoved OnInventoryItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FInventoryBroadcastComplete InventoryBroadcastComplete;

	UPROPERTY(BlueprintAssignable)
	FScrollBoxResetSignature ScrollBoxResetDelegate;
	
	void SetOwningActor(AActor* InOwner);

	void BindCallbacksToDependencies();
	void BroadcastInitialValues();
	
private:

	UPROPERTY()
	TObjectPtr<AActor> OwningActor;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> OwningInventory;

	void BroadcastInventoryContents();
};
