// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\UI\WheelUIUserWidget.h"

#include "UI/Widget/PackageItemUserWidget.h"

void UWheelUIUserWidget::AddPackageItemWidget(ASceneItemActor* SceneItemActor)
{
	if (!ItemWidgetClass)
	{
		ItemWidgetClass = LoadClass<UPackageItemUserWidget>(this, *PackageItemPath);
	}
	UPackageItemUserWidget* ItemUserWidget = CreateWidget<UPackageItemUserWidget>(GetOwningPlayer(), ItemWidgetClass);
	// ItemUserWidget->SetItemID(SceneItemActor->GetItemID());
	
}

