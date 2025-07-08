// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PackageUserWidget.h"

#include "Character/DoubleHeroesBaseCharacter.h"
#include "Net/UnrealNetwork.h"
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
			// if(PackageItemMap.Num() <= 0)
			// {
			// 	AddItemWidgetToMap();
			// }
			// PackageItemMap = Character->GetPackageComponent()->PackageMap;
			ItemSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>();
			
			//把所有的物品都显示出来
			for (int32 i = 0; i < 4; i++)
			{
				if(PackageItemMap.Contains(i))
				{
					PackageItemMap[i]->IconImage->SetBrushFromTexture(ItemSubsystem->GetItemData(PackageItemMap[i]->ItemId)->Icon);
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

void UPackageUserWidget::OpenPackageUI()
{
	AddItemWidgetToMap();
	AddToViewport();
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

void UPackageUserWidget::OnPutOnItem(int32 index)
{

	//穿戴装备
	if (ADoubleHeroesBaseCharacter* Player = Cast<ADoubleHeroesBaseCharacter>(GetOwningPlayerPawn()))
	{
		if (Player->GetPackageComponent())
		{
			if (Player->GetPackageComponent()->PackageMap.Contains(index))
			{
				//把角色当前身上的装备Attach到Socket上
				
			}
		}
	}
	
	ActiveSlotIndex = index;
}

void UPackageUserWidget::OnTakeOffItem(int32 ItemID)
{
}

void UPackageUserWidget::AddItemWidgetToMap()
{
	// PackageItem_0->SlotIndex = 0;
	// PackageItem_1->SlotIndex = 1;
	// PackageItem_2->SlotIndex = 2;
	// PackageItem_3->SlotIndex = 3;
	// PackageItemMap.Add(0, PackageItem_0);
	// PackageItemMap.Add(1, PackageItem_1);
	// PackageItemMap.Add(2, PackageItem_2);
	// PackageItemMap.Add(3, PackageItem_3);
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

void UPackageUserWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;

	//蓝图绑定
	OnWidgetControllerSet();
}



