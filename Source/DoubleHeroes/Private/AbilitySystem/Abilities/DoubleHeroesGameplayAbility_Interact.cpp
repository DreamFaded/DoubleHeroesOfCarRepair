// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DoubleHeroesGameplayAbility_Interact.h"

#include "AbilitySystem/AbilityTasks/AbilityTask_GrantNearbyInteraction.h"
#include "IndicatorSystem/DoubleHeroesIndicatorManagerComponent.h"
#include "IndicatorSystem/IndicatorDescriptor.h"
#include "Interaction/InteractionOption.h"
#include "Interaction/InteractionStatics.h"
#include "Player/DoubleHeroesPlayerController.h"

UDoubleHeroesGameplayAbility_Interact::UDoubleHeroesGameplayAbility_Interact(
	const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = EDoubleHeroesAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UDoubleHeroesGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UDoubleHeroesGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)
{
	if (ADoubleHeroesPlayerController* PC = GetDoubleHeroesPlayerControllerFromActorInfo())
	{
		if (UDoubleHeroesIndicatorManagerComponent* IndicatorManager = UDoubleHeroesIndicatorManagerComponent::GetComponent(PC))
		{
			for (UIndicatorDescriptor* Indicator : Indicators)
			{
				IndicatorManager->RemoveIndicator(Indicator);
			}
			Indicators.Reset();

			for (const FInteractionOption& InteractionOption : InteractiveOptions)
			{
				AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

				TSoftClassPtr<UUserWidget> InteractionWidgetClass = 
					InteractionOption.InteractionWidgetClass.IsNull() ? DefaultInteractionWidgetClass : InteractionOption.InteractionWidgetClass;

				UIndicatorDescriptor* Indicator = NewObject<UIndicatorDescriptor>();
				Indicator->SetDataObject(InteractableTargetActor);
				Indicator->SetSceneComponent(InteractableTargetActor->GetRootComponent());
				Indicator->SetIndicatorClass(InteractionWidgetClass);
				IndicatorManager->AddIndicator(Indicator);

				Indicators.Add(Indicator);
			}
		}
		else
		{
			//TODO This should probably be a noisy warning.  Why are we updating interactions on a PC that can never do anything with them?
		}
	}

	CurrentOptions = InteractiveOptions;
}

void UDoubleHeroesGameplayAbility_Interact::TriggerInteraction()
{
}
