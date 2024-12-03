// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PackageItemUserWidget.generated.h"

class UTextBlock;
class UImage;
class ASceneItemActor;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;
	
	virtual void NativeConstruct() override;

	void InitPanel(ASceneItemActor* SceneItemActor);


private:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

};
