// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//传参到蓝图
	OnHealthChanged.Broadcast(GetDoubleHeroesAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetDoubleHeroesAS()->GetMaxHealth());
	OnEnduranceChanged.Broadcast(GetDoubleHeroesAS()->GetEndurance());
	OnMaxEnduranceChanged.Broadcast(GetDoubleHeroesAS()->GetMaxEndurance());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDoubleHeroesAS()->GetHealthAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnHealthChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDoubleHeroesAS()->GetMaxHealthAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnMaxHealthChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetDoubleHeroesAS()->GetEnduranceAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnEnduranceChanged.Broadcast(Data.NewValue);
	                        });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                        GetDoubleHeroesAS()->GetMaxEnduranceAttribute()).
	                        AddLambda([this](const FOnAttributeChangeData& Data)
	                        {
		                        OnMaxEnduranceChanged.Broadcast(Data.NewValue);
	                        });

	if (UDHAbilitySystemComponent* DoubleHeroesASC = Cast<UDHAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (DoubleHeroesASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(DoubleHeroesASC);
		}
		else
		{
			DoubleHeroesASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		DoubleHeroesASC->EffectAssetTags.AddLambda(
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

	

	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UDHAbilitySystemComponent* DoubleHeroesASC)
{
	//Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	if(!DoubleHeroesASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, DoubleHeroesASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		//Need a way to figure out the ability tag for a given ability spec.
		FDoubleHeroesAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(DoubleHeroesASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = DoubleHeroesASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	DoubleHeroesASC->ForEachAbility(BroadcastDelegate);
}

