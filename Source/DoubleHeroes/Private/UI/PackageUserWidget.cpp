// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PackageUserWidget.h"

#include "Character/DoubleHeroesBaseCharacter.h"
#include "Subsystem/ItemSubsystem.h"
#include "UI/WheelUIUserWidget.h"
#include "UI/Widget/PackageItemUserWidget.h"



void UPackageUserWidget::ShowUI()
{
	if(ADoubleHeroesBaseCharacter* Character = Cast<ADoubleHeroesBaseCharacter>(GetOwningPlayerPawn()))
	{
		if (Character->GetPackageComponent())
		{
			// Character->GetPackageComponent()->OnAddNearItem.AddUObject(this, &UPackageComponent::OnAddNearItem);
			// Character->GetPackageComponent()->OnRemoveNearItem.AddUObject(this, &UPackageComponent::OnRemoveNearItem);
			//
			// Character->GetPackageComponent()->OnAddItemToPackage.AddUObject(this, &UPackageUserWidget::OnAddPackageItem);
			// Character->GetPackageComponent()->OnRemoveItemFromPackage.AddUObject(this, &UPackageUserWidget::OnRemovePackageItem);
			if(PackageItemArray.Num() <= 0)
			{
				AddItemWidgetToArray();
			}
			PackageItemMap = Character->GetPackageComponent()->PackageMap;
			ItemSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>();
			PackageItemMap.Add(1,1);
			PackageItemMap.Add(2,2);
			for (int32 i = 0; i <= 3; i++)
			{
				if(PackageItemMap.Contains(i+1))
				{
					PackageItemArray[i]->CurrentItemId = PackageItemMap[i+1];
					PackageItemArray[i]->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(i+1)->Icon);
				}
			}
			// if(PackageItemMap.Contains(1))
			// {
			// 	PackageItem_1->CurrentItemId = PackageItemMap[1];
			// 	PackageItem_1->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(1)->Icon);
			// }
			// if(PackageItemMap.FindKey(2))
			// {
			// 	PackageItem_2->CurrentItemId = PackageItemMap[2];
			// 	PackageItem_2->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(2)->Icon);
			// }
			// if(PackageItemMap.FindKey(3))
			// {
			// 	PackageItem_2->CurrentItemId = PackageItemMap[3];
			// 	PackageItem_2->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(3)->Icon);
			// }
			// if(PackageItemMap.FindKey(4))
			// {
			// 	PackageItem_2->CurrentItemId = PackageItemMap[4];
			// 	PackageItem_2->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(4)->Icon);
			// }
		}
	}
}

void UPackageUserWidget::AddPackageItemWidget(int32 Sign, int32 ItemID)
{
	if (ItemWidgetClass)
	{
		ItemWidgetClass = LoadClass<UPackageItemUserWidget>(nullptr, TEXT("/Game/Blueprints/UI/Package/WBP_PackageItem.WBP_PackageItem_C"));
	}
	UPackageItemUserWidget* ItemUserWidget = CreateWidget<UPackageItemUserWidget>(GetOwningPlayer(), ItemWidgetClass);
	// ItemUserWidget->InitPanel(Sign, ItemID);
	
}

void UPackageUserWidget::RemoveItemWidget(int32 Sign, int32 ItemID)
{
}

void UPackageUserWidget::AddItemWidgetToArray()
{
	PackageItemArray.Add(PackageItem_1);
	PackageItemArray.Add(PackageItem_2);
	PackageItemArray.Add(PackageItem_3);
	PackageItemArray.Add(PackageItem_4);
}

void UPackageUserWidget::OnPutOnItem(ESkinPartType PartType, int32 ItemID)
{
	
}

void UPackageUserWidget::OnTakeOffItem(ESkinPartType PartType, int32 ItemID)
{
}

void UPackageUserWidget::RemoveFromParent()
{
	Super::RemoveFromParent();

	if(ADoubleHeroesBaseCharacter* Player = Cast<ADoubleHeroesBaseCharacter>(GetOwningPlayerPawn()))
	{
		if (Player->GetPackageComponent())
		{
			//解绑通知
			// Player->GetPackageComponent()->AddNearItem.RemoveAll(this);
			// Player->GetPackageComponent()->OnPutOnItem.RemoveAll(this);
			// Player->GetPackageComponent()->OnTakeOffItem.RemoveAll(this, &UPackageUserWidget::OnTakeOffItem);

			
		}
	}
}



