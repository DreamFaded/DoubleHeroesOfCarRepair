// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DoubleHeroesUserWidget.h"

void UDoubleHeroesUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
