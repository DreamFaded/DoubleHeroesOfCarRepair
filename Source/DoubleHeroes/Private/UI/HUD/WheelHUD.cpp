// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\UI\HUD\WheelHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/EquipUIUserWidget.h"
#include "UI/PackageUserWidget.h"
#include "UI/WheelUIUserWidget.h"

AWheelHUD::AWheelHUD()
{
	EquipUIPtr = nullptr;
	WheelUIPtr = nullptr;
	
	bIsOpenWheelUI = false;  
}

void AWheelHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!EquipUIPtr)
	{
		EquipUIPtr = CreateWidget<UEquipUIUserWidget>(GetOwningPlayerController(),
			LoadClass<UEquipUIUserWidget>(nullptr, *EquipUIPath));
	}
	if (EquipUIPtr)
	{
		EquipUIPtr->AddToViewport();
	}
}

void AWheelHUD::OpenWheelUI()
{
	if (!bIsOpenWheelUI)
	{
		if(!WheelUIPtr)
		{
			WheelUIPtr = CreateWidget<UWheelUIUserWidget>(GetOwningPlayerController(),
				LoadClass<UWheelUIUserWidget>(nullptr, *WheelUIPath));
		}
		if (WheelUIPtr)
		{
			WheelUIPtr->AddToViewport();
			bIsOpenWheelUI = true;
		}
		
	}
	else
	{
		WheelUIPtr->RemoveFromParent();
		bIsOpenWheelUI = false;
	}
}

void AWheelHUD::TogglePackageUI()
{
	if (!PackageUserWidget)
	{
		// TSubclassOf<UPackageUserWidget> PackageClass = LoadClass<UPackageUserWidget>(nullptr, *PackageUIPath);
		PackageUserWidget = CreateWidget<UPackageUserWidget>(GetOwningPlayerController(), 
LoadClass<UWheelUIUserWidget>(nullptr, *WheelUIPath));
	}
	if (PackageUserWidget)
	{
		if (PackageUserWidget->IsInViewport())
		{
			PackageUserWidget->RemoveFromParent();
		}
		else
		{
			PackageUserWidget->AddToViewport();
		}
		GetOwningPlayerController()->bShowMouseCursor = PackageUserWidget->IsInViewport();
	}
	
}
