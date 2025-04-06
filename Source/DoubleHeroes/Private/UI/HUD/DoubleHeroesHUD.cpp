// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DoubleHeroesHUD.h"
#include "UI/DoubleHeroesUserWidget.h"
#include "UI/PackageUserWidget.h"
#include "UI/Widget/PackageItemUserWidget.h"

void ADoubleHeroesHUD::TogglePackageUI()
{
	if (!PackageUserWidget)
	{
		TSubclassOf<UPackageUserWidget> PackageClass = LoadClass<UPackageUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Wheel/WBP_PackageUserWidget.WBP_PackageUserWidget_C'"));
		PackageUserWidget = CreateWidget<UPackageUserWidget>(GetOwningPlayerController(), PackageClass);
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

void ADoubleHeroesHUD::OpenPackageUI()
{
	if (!PackageUserWidget)
	{
		TSubclassOf<UPackageUserWidget> PackageClass = LoadClass<UPackageUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Wheel/WBP_PackageUserWidget.WBP_PackageUserWidget_C'"));
		PackageUserWidget = CreateWidget<UPackageUserWidget>(GetOwningPlayerController(), PackageClass);
	}
		FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
	if (PackageUserWidget)
	{
		GetOwningPlayerController()->bShowMouseCursor = true;
		PackageUserWidget->ShowUI();
		// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PackageUserWidget->AddToViewport();
	}
}

void ADoubleHeroesHUD::ClosePackageUI()
{
	FInputModeGameOnly InputModeGame;
	GetOwningPlayerController()->SetInputMode(InputModeGame);
	if (PackageUserWidget)
	{
		if (PackageUserWidget->IsInViewport())
		{
			TArray<UPackageItemUserWidget*> ItemArray = PackageUserWidget->PackageItemArray;
			for(int32 i = 0; i < ItemArray.Num(); i++)
			{
				if(ItemArray[i] && ItemArray[i]->CurrentItemId > 0)
				{
					PackageUserWidget->OnPutOnItem(i);
					return;
				}
			}
			PackageUserWidget->RemoveFromParent();
			GetOwningPlayerController()->bShowMouseCursor = false;
		}
	}
}

UOverlayWidgetController* ADoubleHeroesHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ADoubleHeroesHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if(AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
		
		return AttributeMenuWidgetController;
	}
	return AttributeMenuWidgetController;
}

void ADoubleHeroesHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                                   UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_DoubleHeroesHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_DoubleHeroesHUD"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UDoubleHeroesUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
