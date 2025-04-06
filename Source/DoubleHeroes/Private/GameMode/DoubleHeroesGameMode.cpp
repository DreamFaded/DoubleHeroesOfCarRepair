// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DoubleHeroesGameMode.h"

UCharacterClassInfo* ADoubleHeroesGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}

UProjectileInfo* ADoubleHeroesGameMode::GetProjectileInfo() const
{
	return ProjectileInfo;
}
