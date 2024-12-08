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
		int32 Sign = GetPackageMapSign();
		PackageMap.Add(GetPackageMapSign(), ItemActor->GetItemID());

		if (OnAddItemToPackage.IsBound())
		{
			OnAddItemToPackage.Broadcast(Sign, ItemActor->GetItemID());
		}
		// ItemActor->Destroy();
	}
}

void UPackageComponent::RemoveItemFromPackage(ASceneItemActor* ItemActor)
{
	if (ItemActor)
	{
		PackageMap.Remove(ItemActor->GetItemID());
	}
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
