// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PackageItemUserWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/SceneItemActor.h"
#include "Subsystem/ItemSubsystem.h"



void UPackageItemUserWidget::InitPanel(ASceneItemActor* SceneItemActor)
{
	if (SceneItemActor)
	{
		if(FItemBase* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(SceneItemActor->GetItemID()))
		{
			// NameTextBlock->SetText(ItemBase->Name);
			IconImage->SetBrushFromTexture(ItemBase->Icon);
			BindSceneItem = SceneItemActor;
		}
		
	}
}

void UPackageItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetRenderScale(FVector2D(1.5f, 1.5f));
	bIsActiveSlot = true;
	// if(BindSceneItem)
	// {
	// 	CurrentItemId = BindSceneItem->ID;
	// }
}

void UPackageItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetRenderScale(FVector2D(1.f, 1.f));
	bIsActiveSlot = false;
	// CurrentItemId = 0;
}