// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DoubleHeroesSystemsWidget.h"

void UDoubleHeroesSystemsWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
