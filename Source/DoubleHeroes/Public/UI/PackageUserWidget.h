﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Subsystem/ItemSubsystem.h"
#include "PackageUserWidget.generated.h"

class UPackageItemUserWidget;
class UWheelUIUserWidget;
class UEquipUIUserWidget;
class ASceneItemActor;
class UPackageScrollWidget;
class UItemSubsystem;

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// UPROPERTY(meta = (BindWidget))
	// UEquipUIUserWidget* EquipUIUserWidget;
	UPROPERTY(meta = (BindWidget))
	UPackageItemUserWidget* PackageItem_1;
	UPROPERTY(meta = (BindWidget))
	UPackageItemUserWidget* PackageItem_2;
	UPROPERTY(meta = (BindWidget))
	UPackageItemUserWidget* PackageItem_3;
	UPROPERTY(meta = (BindWidget))
	UPackageItemUserWidget* PackageItem_4;

	TMap<int32, int32> PackageItemMap;

	TArray<UPackageItemUserWidget*> PackageItemArray;

	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;
	
	UItemSubsystem* ItemSubsystem;
	
	void ShowUI();

	void AddPackageItemWidget(int32 Sign, int32 ItemID);
	void RemoveItemWidget(int32 Sign, int32 ItemID);

	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	// void AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item);
	//
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	// ULyraInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	void OnPutOnItem(int32 ItemID);
	void OnTakeOffItem(int32 ItemID);

	void AddItemWidgetToArray();

	
	virtual void RemoveFromParent() override;
protected:
	UClass* ItemWidgetClass;

	void OnPutOnItem(ESkinPartType PartType, int32 ItemID);
	void OnTakeOffItem(ESkinPartType PartType, int32 ItemID);

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

	UFUNCTION()
	void OnRep_Slots();


	

};
