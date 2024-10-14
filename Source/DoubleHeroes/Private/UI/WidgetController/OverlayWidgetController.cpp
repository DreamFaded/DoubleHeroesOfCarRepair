// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DoubleHeroesAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet = CastChecked<UDoubleHeroesAttributeSet>(AttributeSet);
	//传参到蓝图
	OnHealthChanged.Broadcast(DoubleHeroesAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(DoubleHeroesAttributeSet->GetMaxHealth());
	OnEnduranceChanged.Broadcast(DoubleHeroesAttributeSet->GetEndurance());
	OnMaxEnduranceChanged.Broadcast(DoubleHeroesAttributeSet->GetMaxEndurance());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet = CastChecked<UDoubleHeroesAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetHealthAttribute()).
							AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetMaxHealthAttribute()).
							AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetEnduranceAttribute()).
							AddUObject(this, &UOverlayWidgetController::EnduranceChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetMaxEnduranceAttribute()).
							AddUObject(this, &UOverlayWidgetController::MaxEnduranceChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::EnduranceChanged(const FOnAttributeChangeData& Data)
{
	OnEnduranceChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxEnduranceChanged(const FOnAttributeChangeData& Data)
{
	OnMaxEnduranceChanged.Broadcast(Data.NewValue);
}

