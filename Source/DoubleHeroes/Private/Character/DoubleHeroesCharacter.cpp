﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/DoubleHeroesPlayerController.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/HUD/DoubleHeroesHUD.h"


// Sets default values
ADoubleHeroesCharacter::ADoubleHeroesCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ADoubleHeroesCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	InitAbilityActorInfo();
}

void ADoubleHeroesCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ADoubleHeroesCharacter::InitAbilityActorInfo()
{
	ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>();
	check(DoubleHeroesPlayerState);
	DoubleHeroesPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DoubleHeroesPlayerState, this);
	AbilitySystemComponent = DoubleHeroesPlayerState->GetAbilitySystemComponent();
	AttributeSet = DoubleHeroesPlayerState->GetAttributeSet();

	if (ADoubleHeroesPlayerController* DoubleHeroesPlayerController = Cast<ADoubleHeroesPlayerController>(GetController()))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(DoubleHeroesPlayerController->GetHUD()))
		{
			DoubleHeroesHUD->InitOverlay(DoubleHeroesPlayerController, DoubleHeroesPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}