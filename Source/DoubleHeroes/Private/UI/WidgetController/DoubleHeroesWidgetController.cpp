// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DoubleHeroesWidgetController.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Data/AbilityInfo.h"
#include "Player/DoubleHeroesPlayerState.h"

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

void UDoubleHeroesWidgetController::BroadcastAbilityInfo()
{
	if (!GetDoubleHeroesASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FDoubleHeroesAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(DoubleHeroesASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = DoubleHeroesASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = DoubleHeroesASC->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetDoubleHeroesASC()->ForEachAbility(BroadcastDelegate);
}

ADoubleHeroesPlayerState* UDoubleHeroesWidgetController::GetDoubleHeroesPS()
{
	if (DoubleHeroesPlayerState == nullptr)
	{
		DoubleHeroesPlayerState = Cast<ADoubleHeroesPlayerState>(PlayerState);
	}
	return DoubleHeroesPlayerState;
}

UDHAbilitySystemComponent* UDoubleHeroesWidgetController::GetDoubleHeroesASC()
{
	if(DoubleHeroesASC == nullptr)
	{
		DoubleHeroesASC = Cast<UDHAbilitySystemComponent>(AbilitySystemComponent);
	}
	return DoubleHeroesASC;
}

UDoubleHeroesAttributeSet* UDoubleHeroesWidgetController::GetDoubleHeroesAS()
{
	if (DoubleHeroesAttributeSet == nullptr)
	{
		DoubleHeroesAttributeSet = Cast<UDoubleHeroesAttributeSet>(AttributeSet);
	}
	return DoubleHeroesAttributeSet;
}
