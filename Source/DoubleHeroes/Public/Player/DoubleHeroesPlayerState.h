// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "DoubleHeroesPlayerState.generated.h"


class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ADoubleHeroesPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UDHAbilitySystemComponent* GetDHAbilitySystemComponent() const;
	void BindCallbacksToDependencies();
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };

	FORCEINLINE int32 GetPlayerLevel() const {return Level;}

	UFUNCTION(BlueprintPure)
	UDoubleHeroesAttributeSet* GetDoubleHeroesAttributes() const;

	

protected:
	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDHAbilitySystemComponent> DHAbilitySystemComp;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDoubleHeroesAttributeSet> DoubleHeroesAttributes;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
