// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
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
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnHealthChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetMaxHealthAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnMaxHealthChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAttributeSet->GetEnduranceAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnEnduranceChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                        DoubleHeroesAttributeSet->GetMaxEnduranceAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnMaxEnduranceChanged.Broadcast(Data.NewValue);
	                        });

	Cast<UDHAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//For example, say that Tag = Message.HealthPotion
				//"Message.HealthPotion".MatchesTag("Meage") will return true, "Message".MatchesTag("Message.HealthPotion") will return false
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
}

