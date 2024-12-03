// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PackageUserWidget.h"

#include "Character/DoubleHeroesBaseCharacter.h"
#include "UI/WheelUIUserWidget.h"

void UPackageUserWidget::ShowUI()
{
	if(ADoubleHeroesBaseCharacter* Character = Cast<ADoubleHeroesBaseCharacter>(GetOwningPlayerPawn()))
	{
		if (Character->GetPackageComponent())
		{
			Character->GetPackageComponent()->OnAddNearItem.AddUObject(this, &UPackageUserWidget::OnAddNearItem);
			Character->GetPackageComponent()->OnRemoveNearItem.AddUObject(this, &UPackageUserWidget::OnRemoveNearItem);
		}
	}
	AddToViewport();
}

void UPackageUserWidget::OnAddNearItem(ASceneItemActor* SceneItemActor)
{
	WheelUIUserWidget->AddPackageItemWidget(SceneItemActor);
}

void UPackageUserWidget::OnRemoveNearItem(ASceneItemActor* SceneItemActor)
{
	
}
