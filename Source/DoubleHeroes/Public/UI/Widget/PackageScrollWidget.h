// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageScrollWidget.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageScrollWidget : public UUserWidget
{
	GENERATED_BODY()

	void AddItem(int32 ItemID);

protected:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* MyScrollBox;
};
