// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController//DoubleHeroesWidgetController.h"

void UDoubleHeroesWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UDoubleHeroesWidgetController::BroadcastInitialValues()
{
}

void UDoubleHeroesWidgetController::BindCallbacksToDependencies()
{
}
