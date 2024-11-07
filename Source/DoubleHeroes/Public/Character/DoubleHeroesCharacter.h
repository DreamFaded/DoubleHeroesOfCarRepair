// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesCharacterBase.h"
#include "DoubleHeroesCharacter.generated.h"

struct FInputActionValue;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UDataAsset_InputConfig;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesCharacter : public ADoubleHeroesCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Combat Interface
	virtual int32 GetPlayerLevel_Implementation();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

};
