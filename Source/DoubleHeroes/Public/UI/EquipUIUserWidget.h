// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/ItemSubsystem.h"
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
	
	void InitPanel(int32 ItemID = -1);
	ESkinPartType GetPartType() const { return PartType; }
protected:

	UPROPERTY(EditAnywhere)
	ESkinPartType PartType;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY()
	TArray<UTexture2D*> IconArray;

	virtual void NativePreConstruct() override;


	
};
