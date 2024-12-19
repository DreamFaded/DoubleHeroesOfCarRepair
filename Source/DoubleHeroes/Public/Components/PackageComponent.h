// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Subsystem/ItemSubsystem.h"
#include "PackageComponent.generated.h"


class ASceneItemActor;

DECLARE_MULTICAST_DELEGATE_OneParam(DelegateNearItem, ASceneItemActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(DelegatePackageItem, int32, int32)
DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateSkinPart, ESkinPartType, int32)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int32, int32> PackageMap;

	TMap<ESkinPartType, int32> SkinPartMap;

	DelegateNearItem OnAddNearItem;
	DelegateNearItem OnRemoveNearItem;

	DelegatePackageItem OnAddItemToPackage;
	DelegatePackageItem OnRemoveItemFromPackage;

	DelegateSkinPart OnPutOnItem;
	DelegateSkinPart OnTakeOffItem;
	
	// Sets default values for this component's properties
	UPackageComponent();

	void AddNearItem(ASceneItemActor* SceneItemActor);
	void RemoveNearItem(ASceneItemActor* SceneItemActor);
	
	void AddItemToPackage(ASceneItemActor* ItemActor);
	void RemoveItemFromPackageToScene(int32 Sign);
	void RemoveItemFromPackage(int32 Sign);

	void PutOnItemFromNear(ASceneItemActor* ItemActor, ESkinPartType SkinPart);
	void PutOnItemFromPackage(int32 PackageSign, ESkinPartType SkinPart);
	bool PutOnItem(int32 ItemID, ESkinPartType SkinPart);

	//脱装备
	void TakeOffItemToPackage(ESkinPartType SkinPartType, bool bNotify = false);
	void TakeOffItemToScene(ESkinPartType SkinPartType, bool bNotify = false);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(int32 ID);

	//怪物掉落道具
	void SpawnNearSceneItem(int32 ItemID);

	AWeapon* GetHoloWeapon() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ASceneItemActor*> NearItems;

	UPROPERTY()
	AWeapon* HoloWeapon;

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
