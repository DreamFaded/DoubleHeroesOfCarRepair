// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"

#include "Character/DoubleHeroesCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/HUD/DoubleHeroesHUD.h"

UOverlayWidgetController* UDoubleHeroesAbilitySystemLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(PC->GetHUD()))
		{
			ADoubleHeroesPlayerState* PS = PC->GetPlayerState<ADoubleHeroesPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DoubleHeroesHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UDoubleHeroesAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(PC->GetHUD()))
		{
			ADoubleHeroesPlayerState* PS = PC->GetPlayerState<ADoubleHeroesPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return DoubleHeroesHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
