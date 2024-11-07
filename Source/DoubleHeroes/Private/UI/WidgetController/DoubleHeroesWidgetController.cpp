// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DoubleHeroesWidgetController.h"

#include "AbilitySystem/DoubleHeroesAttributeSet.h"

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

/*UDHAbilitySystemComponent* UDoubleHeroesWidgetController::GetDoubleHeroesASC()
{
	if (DHAbilitySystemComponent == nullptr)
	{
		DHAbilitySystemComponent = Cast<UDHAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DHAbilitySystemComponent;
}*/

UDoubleHeroesAttributeSet* UDoubleHeroesWidgetController::GetDoubleHeroesAS()
{
	if (DoubleHeroesAttributeSet == nullptr)
	{
		DoubleHeroesAttributeSet = Cast<UDoubleHeroesAttributeSet>(AttributeSet);
	}
	return DoubleHeroesAttributeSet;
}