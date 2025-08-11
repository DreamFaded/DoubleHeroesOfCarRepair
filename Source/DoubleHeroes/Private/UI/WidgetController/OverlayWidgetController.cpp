// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Data/AbilityInfo.h"
#include "Player/DoubleHeroesPlayerState.h"

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
	GetDoubleHeroesPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetDoubleHeroesPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
	});

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

	if (GetDoubleHeroesASC())
	{
		if (GetDoubleHeroesASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetDoubleHeroesASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UDHAbilitySystemComponent* InDHAbilitySystemComponent)
{
	if(!GetDoubleHeroesASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		//Need a way to figure out the ability tag for a given ability spec.
		FDoubleHeroesAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(DoubleHeroesASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = DoubleHeroesASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetDoubleHeroesASC()->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::BroadcastAbilityInfo()
{
	//Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	if(!DoubleHeroesASC->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		//Need a way to figure out the ability tag for a given ability spec.
		FDoubleHeroesAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(DoubleHeroesASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = DoubleHeroesASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = DoubleHeroesASC->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetDoubleHeroesASC()->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	// const ULevelUpInfo* LevelUpInfo = GetDoubleHeroesPS()->LevelUpInfo;
	// checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));
	//
	// const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	// const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	//
	// if (Level <= MaxLevel && Level > 0)
	// {
	// 	const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
	// 	const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
	//
	// 	const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
	// 	const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
	//
	// 	const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
	//
	// 	OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	// }
}

