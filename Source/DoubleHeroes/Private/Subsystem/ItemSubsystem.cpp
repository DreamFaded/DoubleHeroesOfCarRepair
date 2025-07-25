// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ItemSubsystem.h"

FItemData* UItemSubsystem::GetItemData(int32 ID) const
{
	if (ItemMap.Contains(ID))
	{
		return ItemMap[ID];
	}
	return nullptr;
}

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//加载数据表格
	ItemData = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Data/DT_SkinPart.DT_SkinPart'"));

	if (ItemData)
	{
		TArray<FName> RowNames = ItemData->GetRowNames();
		for (auto Key : RowNames)
		{
			FItemData* ItemBase = reinterpret_cast<FItemData*>(ItemData->GetRowMap()[Key]);
			FString Sign = Key.ToString();
			FString LeftStr;
			FString RightStr;
			int32 ID = 0;
			if(Sign.Split(TEXT("_"), &LeftStr, &RightStr))
			{
				//字符转整型
				ID = FCString::Atoi(*RightStr) + 1;
			}

			if (!ItemMap.Contains(ID))
			{
				ItemMap.Add(ID, ItemBase);//添加元素
			}
		}
	}

	//加载武器表格
	WeaponData = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Data/DT_WeaponInfo.DT_WeaponInfo''"));

	if (WeaponData)
	{
		TArray<FName> RowNames = WeaponData->GetRowNames();
		for (auto Key : RowNames)
		{
			FItemData* ItemBase = reinterpret_cast<FItemData*>(WeaponData->GetRowMap()[Key]);
			FString Sign = Key.ToString();
			FString LeftStr;
			FString RightStr;
			int32 ID = WEAPONINDEX;
			if(Sign.Split(TEXT("_"), &LeftStr, &RightStr))
			{
				//字符转整型
				ID = FCString::Atoi(*RightStr) + 1 + WEAPONINDEX;
			}

			if (!ItemMap.Contains(ID))
			{
				ItemMap.Add(ID, ItemBase);//添加元素
			}
		}
	}
}
