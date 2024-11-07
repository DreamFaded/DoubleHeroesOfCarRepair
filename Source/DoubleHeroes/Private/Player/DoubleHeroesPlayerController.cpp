// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Character/DoubleHeroesCharacter.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/DamageTextComponent.h"

ADoubleHeroesPlayerController::ADoubleHeroesPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

void ADoubleHeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void ADoubleHeroesPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ADoubleHeroesPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ADoubleHeroesPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if(LastActor) LastActor->UnHighlightActor();
		if(ThisActor) ThisActor->HighlightActor();
	}
	/* A.LastActor is null && ThisActor is null
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

	/*if (LastActor == nullptr)
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
	}*/
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
	if (!InputTag.MatchesTagExact(FDoubleHeroesGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting || !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					// DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				// 防止点击到NavMeshBoundsVolume以外的地方时因为数组越界崩溃
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
				else
				{
					// 处理空路径的情况
					UE_LOG(LogTemp, Warning, TEXT("No path points found"));
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
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

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
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
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//不隐藏光标
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ADoubleHeroesPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(InputComponent);
	DoubleHeroesInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
	                                       &ADoubleHeroesPlayerController::Move);
	DoubleHeroesInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this,
	&ADoubleHeroesPlayerController::Run);
	DoubleHeroesInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this,
	&ADoubleHeroesPlayerController::Walk);
	
	DoubleHeroesInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
	                                       &ADoubleHeroesPlayerController::Look);
	DoubleHeroesInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this,
	&ADoubleHeroesPlayerController::ShiftPressed);
	DoubleHeroesInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this,
	&ADoubleHeroesPlayerController::ShiftReleased);*/
}

/*
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

void ADoubleHeroesPlayerController::Run(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Run"));
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//设置玩家移动速度为3200
		bIsRunning = true;
		Cast<ADoubleHeroesCharacter>(ControlledPawn)->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;

	}
}

void ADoubleHeroesPlayerController::Walk(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Walk"));
		bIsRunning = false;
		Cast<ADoubleHeroesCharacter>(ControlledPawn)->GetCharacterMovement()->MaxWalkSpeed = 600.0f;

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
}*/


