// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacter.h"

#include "AbilitySystemComponent.h"
#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
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
	AddCharacterAbilities();
}

void ADoubleHeroesCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 ADoubleHeroesCharacter::GetPlayerLevel_Implementation()
{
	const ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>();
	check(DoubleHeroesPlayerState);
	return DoubleHeroesPlayerState->GetPlayerLevel();
}

void ADoubleHeroesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);
	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(PlayerInputComponent);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

}

void ADoubleHeroesCharacter::InitAbilityActorInfo()
{
	ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>();
	check(DoubleHeroesPlayerState);
	DoubleHeroesPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DoubleHeroesPlayerState, this);
	Cast<UDHAbilitySystemComponent>(DoubleHeroesPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = DoubleHeroesPlayerState->GetAbilitySystemComponent();
	AttributeSet = DoubleHeroesPlayerState->GetAttributeSet();

	if (ADoubleHeroesPlayerController* DoubleHeroesPlayerController = Cast<ADoubleHeroesPlayerController>(GetController()))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(DoubleHeroesPlayerController->GetHUD()))
		{
			DoubleHeroesHUD->InitOverlay(DoubleHeroesPlayerController, DoubleHeroesPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void ADoubleHeroesCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADoubleHeroesCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (LookAxisVector.X != 0.f)
	{
	AddControllerYawInput(LookAxisVector.X);
	}
	if (LookAxisVector.Y != 0.f)
	{
	AddControllerPitchInput(LookAxisVector.Y);
	}
}
