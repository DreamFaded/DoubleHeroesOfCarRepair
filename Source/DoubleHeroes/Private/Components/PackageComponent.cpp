// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PackageComponent.h"

#include "Item/SceneItemActor.h"


// Sets default values for this component's properties
UPackageComponent::UPackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPackageComponent::AddNearItem(ASceneItemActor* ItemActor)
{
	NearItems.AddUnique(ItemActor);//只添加一次
	if (OnAddNearItem.IsBound())//代理是否存在有效绑定
	{
		OnAddNearItem.Broadcast(ItemActor);
	}
}

void UPackageComponent::RemoveNearItem(ASceneItemActor* SceneItemActor)
{
	NearItems.Remove(SceneItemActor);
	if (OnRemoveNearItem.IsBound())
	{
		OnRemoveNearItem.Broadcast(SceneItemActor);
	}
}




// Called when the game starts
void UPackageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UPackageComponent::Server_AddItemToPackage_Implementation(ASceneItemActor* ItemActor)
{
	AddItemToPackage(ItemActor);
}

bool UPackageComponent::Server_AddItemToPackage_Validate(ASceneItemActor* ItemActor)
{
	return (ItemActor->GetActorLocation() - GetOwner() ->GetActorLocation()).Size() < 500;
}

// Called every frame
void UPackageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UPackageComponent::AddItemToPackage(ASceneItemActor* ItemActor)
{
	if (ItemActor)
	{
		if (!GetOwner()->HasAuthority())
		{
			Server_AddItemToPackage(ItemActor);
			return;
		}
		PackageMap.Add(GetPackageMapSign(), ItemActor->GetItemID());

		if (OnAddItemToPackage.IsBound())
		{
			OnAddItemToPackage.Broadcast(GetPackageMapSign(), ItemActor->GetItemID());
		}
		// ItemActor->Destroy();
	}
}

void UPackageComponent::RemoveItemFromPackageToScene(int32 Sign)
{
	if (PackageMap.Contains(Sign))
	{
		//先向地面添加一个
		SpawnNearSceneItem(PackageMap[Sign]);
		
		RemoveItemFromPackage(Sign);
	}
}

//从背包中移除物品
void UPackageComponent::RemoveItemFromPackage(int32 Sign)
{
	if (PackageMap.Contains(Sign))
	{
		if (OnRemoveItemFromPackage.IsBound())
		{
			OnRemoveItemFromPackage.Broadcast(Sign, PackageMap[Sign]);
		}
		PackageMap.Remove(Sign);
	}
}

void UPackageComponent::PutOnItemFromNear(ASceneItemActor* ItemActor, ESkinPartType SkinPart)
{
	if(ItemActor && PutOnItem(ItemActor->GetItemID(), SkinPart))
	{
		ItemActor->Destroy();
	}
}

void UPackageComponent::PutOnItemFromPackage(int32 PackageSign, ESkinPartType SkinPart)
{
	if (PackageMap.Contains(PackageSign) && PutOnItem(PackageMap[PackageSign], SkinPart))
	{
		RemoveItemFromPackage(PackageSign);
	}
}

bool UPackageComponent::PutOnItem(int32 ItemID, ESkinPartType SkinPart)
{
	//获取数据
	FItemData* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(ItemID);
	//如果穿戴不在数据表或者不是皮肤组件
	if (!ItemBase || ItemBase->Type == EItemType::EPT_Weapon)
	{
		return false;
	}
	FSkinPart* SkinPartData = static_cast<FSkinPart*>(ItemBase);
	if (SkinPart != ESkinPartType::ESPT_None)
	{
		//拖拽的和想穿的位置不一致
		if (SkinPartData->PartType != SkinPart)
		{
			return false;
		}
	}
	else
	{
		//执行else说明在给AI穿装备
		SkinPart = SkinPartData->PartType;
	}
	//先脱后穿
	TakeOffItemToPackage(SkinPart);
	//穿装备
	SkinPartMap.Add(SkinPart, ItemID);
	//做通知
	if (OnPutOnItem.IsBound())
	{
		OnPutOnItem.Broadcast(SkinPart, ItemID);
	}
	return true;
}

void UPackageComponent::TakeOffItemToPackage(ESkinPartType SkinPartType, bool bNotify)
{
	//先检查是否有道具
	if (SkinPartMap.Contains(SkinPartType))
	{
		int32 ItemID = SkinPartMap[SkinPartType];
		SkinPartMap.Remove(SkinPartType);
		//放到包里
		int32 Sign = GetPackageMapSign();
		PackageMap.Add(Sign, ItemID);
		if (OnAddItemToPackage.IsBound())
		{
			OnAddItemToPackage.Broadcast(Sign, ItemID);
		}

		if (bNotify)
		{
			//通知脱掉部件了
			if (OnTakeOffItem.IsBound())
			{
				OnTakeOffItem.Broadcast(SkinPartType, ItemID);
			}
		}
	}
}

void UPackageComponent::TakeOffItemToScene(ESkinPartType SkinPartType, bool bNotify)
{
	//检查是否有道具
	if (SkinPartMap.Contains(SkinPartType))
	{
		int32 ID = SkinPartMap[SkinPartType];
		SkinPartMap.Remove(SkinPartType);
		//添加到场景
		SpawnNearSceneItem(ID);
		if (bNotify)
		{
			//通知脱掉部件了
			if (OnTakeOffItem.IsBound())
			{
				OnTakeOffItem.Broadcast(SkinPartType, ID);
			}
		}
	}
}

void UPackageComponent::EquipWeapon(int32 ID)
{
	// if (HoldWeapon)
	// {
	// 	UnEquipWeaponToPackage();
	// }
}

void UPackageComponent::SpawnNearSceneItem(int32 ItemID)
{
	FVector Position = GetOwner()->GetActorLocation() + FRotator(0, FMath::FRandRange(0.f, 360.f), 0).Vector() * FMath::FRandRange(20.f, 100.f);
	FTransform Tran;
	Tran.SetLocation(Position);
	ASceneItemActor* ItemActor = GetWorld()->SpawnActorDeferred<ASceneItemActor>(ASceneItemActor::StaticClass(), Tran);
	ItemActor->SetItemID(ItemID);
	ItemActor->FinishSpawning(Tran);
	
}

AItemBase* UPackageComponent::GetHoldWeapon() const
{
	return nullptr;
}

int32 UPackageComponent::GetPackageMapSign()
{
	int32 Index = 0;
	while (PackageMap.Contains(Index))
	{
		Index++;
	}
	return Index;
}
