// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesBaseAnimInstance.h"
#include "GameplayEffectTypes.h"
#include "DoubleHeroesAnimInstance.generated.h"

class UCharacterMovementComponent;
class ADoubleHeroesBaseCharacter;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UDoubleHeroesAnimInstance : public UDoubleHeroesBaseAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	//GAS
	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

protected:

	UPROPERTY()
	ADoubleHeroesBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AnimData||LocomotionData")
	float GroundSpeed;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData||LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "AnimData||LocomotionData")
	bool bSprinting;

	UPROPERTY()
	bool bHoldWeapon;

//private:

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;


	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	//GAS
	UPROPERTY(EditDefaultsOnly, Category="Custom Values|Property Map")
	FGameplayTagBlueprintPropertyMap PropertyMap;
};
