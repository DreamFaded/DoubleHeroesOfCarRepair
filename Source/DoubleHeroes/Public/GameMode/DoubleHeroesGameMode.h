// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DoubleHeroesGameMode.generated.h"

class UProjectileInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;
	UProjectileInfo* GetProjectileInfo() const;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Custom Values| Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;


	UPROPERTY(EditDefaultsOnly, Category = "Custom Values| Class Defaults")
	TObjectPtr<UProjectileInfo> ProjectileInfo;
};
