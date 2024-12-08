// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PackageComponent.generated.h"


class ASceneItemActor;

DECLARE_MULTICAST_DELEGATE_OneParam(DelegateNearItem, ASceneItemActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(DelegatePackageItem, int32, int32)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DelegateNearItem OnAddNearItem;
	DelegateNearItem OnRemoveNearItem;

	DelegatePackageItem OnAddItemToPackage;
	DelegatePackageItem OnRemoveItemFromPackage;
	// Sets default values for this component's properties
	UPackageComponent();

	void AddNearItem(ASceneItemActor* SceneItemActor);
	void RemoveNearItem(ASceneItemActor* SceneItemActor);
	
	void AddItemToPackage(ASceneItemActor* ItemActor);
	void RemoveItemFromPackage(ASceneItemActor* ItemActor);
	
	UPROPERTY(BlueprintReadOnly)
	TMap<int32, int32> PackageMap;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ASceneItemActor*> NearItems;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItemToPackage(ASceneItemActor* SceneItemActor);
	
	// UPROPERTY(meta = (BindWidget))
	// UPackageScrollWidget* NearScrollWidget;
	// UPROPERTY()
	// TArray<ASceneItemActor*> NearItems;
	

	
	int32 GetPackageMapSign();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// TArray<> NearItems;
};
