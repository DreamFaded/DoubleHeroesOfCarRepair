// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Data/AttributeInfo.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"

//获取标签名和参数并回调
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for(auto& Pair : GetDoubleHeroesAS()->TagsToAttributes)
	{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	// GetDoubleHeroesPS()->OnAttributePointsChangedDelegate.AddLambda(
	// 	[this](int32 Points)
	// 	{
	// 		AttributePointsChangedDelegate.Broadcast(Points);
	// 	}
	// );
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
