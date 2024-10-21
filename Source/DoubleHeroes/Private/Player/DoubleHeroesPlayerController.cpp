// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/DoubleHeroesCharacter.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Components/SplineComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Net/UnrealNetwork.h"

ADoubleHeroesPlayerController::ADoubleHeroesPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ADoubleHeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void ADoubleHeroesPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/* A.LastActor is null && ThisActor iss null
	 * do nothing
	 * B.LastActor is null && ThisActor is not null
	 *  Highlight ThisActor
	 * C.LastActor is not null && ThisActor is null
	 *  UnHighlight LastActor
	 * D.Both are valid, but LastActor != ThisActor
	 *  Unhighlight LastActor, Highlight ThisActor
	 * E.D.Both are valid, LastActor == ThisActor
	 *  do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
		else
		{
			//both null 
		}
	}
	else //LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void ADoubleHeroesPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FDoubleHeroesGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *InputTag.ToString());
}

void ADoubleHeroesPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ADoubleHeroesPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FDoubleHeroesGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UDHAbilitySystemComponent* ADoubleHeroesPlayerController::GetASC()
{
	if (DHAbilitySystemComponent == nullptr)
	{
		DHAbilitySystemComponent = Cast<UDHAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return DHAbilitySystemComponent;
}

void ADoubleHeroesPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(DoubleHeroesContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DoubleHeroesContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	//鼠标不锁定到窗口
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ADoubleHeroesPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
	                                   &ADoubleHeroesPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
	                                   &ADoubleHeroesPlayerController::Look);*/
	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(InputComponent);
	DoubleHeroesInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
	                                       &ADoubleHeroesPlayerController::Move);
	DoubleHeroesInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
	                                       &ADoubleHeroesPlayerController::Look);
	DoubleHeroesInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                               &ThisClass::AbilityInputTagReleased,
	                                               &ThisClass::AbilityInputTagHeld);
}

void ADoubleHeroesPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADoubleHeroesPlayerController::Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}
