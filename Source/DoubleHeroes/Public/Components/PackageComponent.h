// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PackageComponent.generated.h"


class ASceneItemActor;

DECLARE_MULTICAST_DELEGATE_OneParam(DelegateNearItem, ASceneItemActor*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API UPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DelegateNearItem OnAddNearItem;
	DelegateNearItem OnRemoveNearItem;
	// Sets default values for this component's properties
	UPackageComponent();

	void AddNearItem(ASceneItemActor* SceneItemActor);
	void RemoveNearItem(ASceneItemActor* SceneItemActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ASceneItemActor*> NearItems;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// TArray<> NearItems;
};
