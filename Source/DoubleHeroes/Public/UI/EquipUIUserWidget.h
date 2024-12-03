// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipUIUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UEquipUIUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	

protected:

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY()
	TArray<UTexture2D*> IconArray;
	
};
