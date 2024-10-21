// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Data/AttributeInfo.h"
#include "DoubleHeroesGameplayTags.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UDoubleHeroesAttributeSet* AS = CastChecked<UDoubleHeroesAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for(auto& Pair : AS->TagsToAttributes)
	{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UDoubleHeroesAttributeSet* AS = CastChecked<UDoubleHeroesAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FDoubleHeroesAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
