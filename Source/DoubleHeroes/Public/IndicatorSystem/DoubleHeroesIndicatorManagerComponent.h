// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "DoubleHeroesIndicatorManagerComponent.generated.h"

class AController;
class UIndicatorDescriptor;
class UObject;
struct FFrame;


UCLASS(BlueprintType, Blueprintable)
class DOUBLEHEROES_API UDoubleHeroesIndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoubleHeroesIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);

	static UDoubleHeroesIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(ULyraIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};
