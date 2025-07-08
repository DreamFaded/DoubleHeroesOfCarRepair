﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "UObject/Interface.h"
#include "InteractableTarget.generated.h"

struct FInteractionQuery;

/**  */
class FInteractionOptionBuilder
{
public:
	FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
	}

	void AddInteractionOption(const FInteractionOption& Option)
	{
		FInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}

private:
	TScriptInterface<IInteractableTarget> Scope;
	TArray<FInteractionOption>& Options;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**  */
class IInteractableTarget
{
	GENERATED_BODY()

public:
	/**  */
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) = 0;

	/**  */
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) { }
};

