// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageUserWidget.generated.h"

class UWheelUIUserWidget;
class UEquipUIUserWidget;
class ASceneItemActor;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UWheelUIUserWidget* WheelUIUserWidget;
	
	void ShowUI();
	
protected:
	
	void OnAddNearItem(ASceneItemActor* SceneItemActor);
	void OnRemoveNearItem(ASceneItemActor* SceneItemActor);
};
