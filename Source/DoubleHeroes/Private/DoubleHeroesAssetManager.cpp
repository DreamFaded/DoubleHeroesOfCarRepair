// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleHeroesAssetManager.h"

#include "DoubleHeroesGameplayTags.h"


UDoubleHeroesAssetManager& UDoubleHeroesAssetManager::Get()
{
	check(GEngine);

	UDoubleHeroesAssetManager* DoubleHeroesAssetManager = Cast<UDoubleHeroesAssetManager>(GEngine->AssetManager);
	return *DoubleHeroesAssetManager;
}

void UDoubleHeroesAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FDoubleHeroesGameplayTags::InitializeNativeGameplayTags();
}
