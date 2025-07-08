// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageItemUserWidget.generated.h"

class UPackageUserWidget;
class UTextBlock;
class UImage;
class ASceneItemActor;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 SlotIndex;
	bool bIsActiveSlot = false;
	int32 ItemId = 0;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;
protected:
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* NameTextBlock;

	void InitPanel(ASceneItemActor* SceneItemActor);

	UPROPERTY()
	ASceneItemActor* BindSceneItem;
	
	int32 PackageSign;


private:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
