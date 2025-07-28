// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DOUBLEHEROES_API DECLARE_LOG_CATEGORY_EXTERN(LogDoubleHeroes, Log, All);
DOUBLEHEROES_API DECLARE_LOG_CATEGORY_EXTERN(LogDoubleHeroesExperience, Log, All);
DOUBLEHEROES_API DECLARE_LOG_CATEGORY_EXTERN(LogDoubleHeroesAbilitySystem, Log, All);
DOUBLEHEROES_API DECLARE_LOG_CATEGORY_EXTERN(LogDoubleHeroesTeams, Log, All);


/**
 * 
 */
DOUBLEHEROES_API FString GetClientServerContextString(UObject* ContextObject = nullptr);