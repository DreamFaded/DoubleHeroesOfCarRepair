﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DHCharacterMovementComponent.h"

#include "Character/BlueHeroCharacter.h"
#include "Character/DoubleHeroesBaseCharacter.h"
#include "Character/DoubleHeroesCharacter.h"


// Sets default values for this component's properties
UDHCharacterMovementComponent::UDHCharacterMovementComponent()
{
	MaxJogSpeed = 300;
	MaxRunSpeed = 800;
}

float UDHCharacterMovementComponent::GetMaxSpeed() const
{
	//引擎的所有移动速度都是从这个方法里拿得的，所以这里重写
	float Speed = Super::GetMaxSpeed();
	if (ADoubleHeroesBaseCharacter* CharacterBase = Cast<ABlueHeroCharacter>(GetOwner()))
	{
		if (CharacterBase->IsRunning())
		{
			Speed = MaxRunSpeed;
		}
	}
	return Speed;
}
