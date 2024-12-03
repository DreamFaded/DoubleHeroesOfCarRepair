// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WheelUIUserWidget.generated.h"

class UPackageItemUserWidget;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UWheelUIUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void AddPackageItemWidget(ASceneItemActor* SceneItemActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PackageItemPath;

	
	
	TSubclassOf<UPackageItemUserWidget> ItemWidgetClass;
};