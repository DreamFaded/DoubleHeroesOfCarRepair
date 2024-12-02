// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PackageItemUserWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/SceneItemActor.h"
#include "Subsystem/ItemSubsystem.h"

void UPackageItemUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 绑定鼠标进入和离开事件
	if (IsValid(this))
	{
		// this->OnMouseEnter.AddDynamic(this, &UPackageItemUserWidget::OnMouseEntera);
		// this->OnMouseLeaveEvent.AddDynamic(this, &UPackageItemUserWidget::OnMouseLeave);
	}
}

void UPackageItemUserWidget::InitPanel(ASceneItemActor* SceneItemActor)
{
	if (SceneItemActor)
	{
		if(FItemBase* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(SceneItemActor->GetItemID()))
		{
			NameTextBlock->SetText(ItemBase->Name);
			IconImage->SetBrushFromTexture(ItemBase->Icon);
		}
		
	}
}

void UPackageItemUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UPackageItemUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}