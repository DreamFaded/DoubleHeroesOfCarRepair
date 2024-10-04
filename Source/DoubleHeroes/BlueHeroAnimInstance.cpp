// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueHeroAnimInstance.h"

#include "BlueHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlueHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlueHeroCharacter = Cast<ABlueHeroCharacter>(TryGetPawnOwner());
}

void UBlueHeroAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlueHeroCharacter == nullptr)
	{
		BlueHeroCharacter = Cast<ABlueHeroCharacter>(TryGetPawnOwner());
	}
	if(BlueHeroCharacter == nullptr) return;

	FVector Velocity = BlueHeroCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bIsInAir = BlueHeroCharacter->GetCharacterMovement()->IsFalling();
}
