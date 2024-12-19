// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/ItemSubsystem.h"
#include "PackageFrameWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UPackageFrameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ESkinPartType GetPartType() const { return PartType; }

	void InitPanel(int32 ItemID = -1);

protected:

	UPROPERTY(EditAnywhere)
	ESkinPartType PartType;

	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
};
