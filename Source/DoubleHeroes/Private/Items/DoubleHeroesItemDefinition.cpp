// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DoubleHeroesItemDefinition.h"

#include "Items/DoubleHeroesItemInstance.h"

// #include UE_INLINE_GENERATED_CPP_BY_NAME(DoubleHeroesItemDefinition)

UDoubleHeroesItemDefinition::UDoubleHeroesItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UDoubleHeroesItemInstance::StaticClass();
}
