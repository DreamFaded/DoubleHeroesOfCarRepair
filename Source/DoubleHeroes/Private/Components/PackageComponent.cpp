// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PackageComponent.h"


// Sets default values for this component's properties
UPackageComponent::UPackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPackageComponent::AddNearItem(ASceneItemActor* SceneItemActor)
{
	NearItems.AddUnique(SceneItemActor);//只添加一次
	if (OnAddNearItem.IsBound())//代理是否存在有效绑定
	{
		OnAddNearItem.Broadcast(SceneItemActor);
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


// Called every frame
void UPackageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

