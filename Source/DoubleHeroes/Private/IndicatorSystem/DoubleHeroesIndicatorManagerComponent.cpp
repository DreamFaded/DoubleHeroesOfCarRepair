// Fill out your copyright notice in the Description page of Project Settings.


#include "IndicatorSystem/DoubleHeroesIndicatorManagerComponent.h"

#include "IndicatorSystem/IndicatorDescriptor.h"


// Sets default values for this component's properties
UDoubleHeroesIndicatorManagerComponent::UDoubleHeroesIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bAutoRegister = true;
	bAutoActivate = true;
}

UDoubleHeroesIndicatorManagerComponent* UDoubleHeroesIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UDoubleHeroesIndicatorManagerComponent>();
	}

	return nullptr;
}

void UDoubleHeroesIndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}

void UDoubleHeroesIndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}

