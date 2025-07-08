// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "BlueHeroAnimInstance.generated.h"

class ADoubleHeroesBaseCharacter;
class ABlueHeroCharacter;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UBlueHeroAnimInstance : public UDoubleHeroesAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData||LocomotionData")
	ABlueHeroCharacter* OwningHeroesCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData||LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData||LocomotionData")
	float EnterRelaxStateThreshold = 5.f;

	float IdleElpasedTime;
	
};
