// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/DoubleHeroesUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "DoubleHeroesHUD.generated.h"

class UPackageUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesHUD : public AHUD
{
	GENERATED_BODY()

public:

	void TogglePackageUI();
	void OpenPackageUI();
	void ClosePackageUI();

	template<typename TWidgetClass>
	TWidgetClass* GetCloneWidget(UClass* OutClass = nullptr);

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

	UPROPERTY()
	UPackageUserWidget* PackageUserWidget;

	UPROPERTY()
	TMap<UClass*, UUserWidget*> CloneWidgetMap;
private:

	UPROPERTY()
	TObjectPtr<UDoubleHeroesUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDoubleHeroesUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};

template <typename TWidgetClass>
TWidgetClass* ADoubleHeroesHUD::GetCloneWidget(UClass* OutClass)
{
	if (!OutClass)
	{
		OutClass = TWidgetClass::StaticClass();
	}
	if (CloneWidgetMap.Contains(OutClass))
	{
		return Cast<TWidgetClass>(CloneWidgetMap[OutClass]);
	}
	else
	{
		TWidgetClass* Widget = CreateWidget<TWidgetClass>(GetOwningPlayerController(), OutClass);
		CloneWidgetMap.Add(OutClass, Widget);
		return Widget;
	}
}