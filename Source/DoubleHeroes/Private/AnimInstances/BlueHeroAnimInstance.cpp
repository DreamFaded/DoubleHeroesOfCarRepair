﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\AnimInstances\BlueHeroAnimInstance.h"

#include "Character/BlueHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlueHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwningCharacter)
	{
		OwningHeroesCharacter = Cast<ABlueHeroCharacter>(OwningCharacter);
	}
}

void UBlueHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (bHasAcceleration || bIsCrouched)
	{
		IdleElpasedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElpasedTime += DeltaSeconds;

		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxStateThreshold);
	}

	if(OwningCharacter == nullptr) return;
	bIsInAir = OwningHeroesCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = OwningHeroesCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;
	bWeaponEquipped = OwningHeroesCharacter->IsWeaponEquipped();
}

