// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/DoubleHeroesUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "DoubleHeroesHUD.generated.h"

struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UDoubleHeroesUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDoubleHeroesUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
