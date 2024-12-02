// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WheelHUD.generated.h"

class UPackageUserWidget;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API AWheelHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EquipUIPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WheelUIPath;

	AWheelHUD();

	virtual void BeginPlay() override;

	void OpenWheelUI();

	void TogglePackageUI();
protected:

	UPROPERTY()
	class UEquipUIUserWidget* EquipUIPtr;

	UPROPERTY()
	class UWheelUIUserWidget* WheelUIPtr;

	UPROPERTY()
	UPackageUserWidget* PackageUserWidget;
	
	bool bIsOpenWheelUI;
};
