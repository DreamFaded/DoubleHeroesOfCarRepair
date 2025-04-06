// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DHAbilitySystemLibrary.generated.h"

struct FDamageEffectInfo;
class UProjectileInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDHAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static UCharacterClassInfo* GetCharacterClassDefaultInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static UProjectileInfo* GetProjectileInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void ApplyDamageEffect(const FDamageEffectInfo& DamageEffectInfo);

	template<typename T>
	static T* GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag);
};

template <typename T>
T* UDHAbilitySystemLibrary::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), FString(""));
}
