// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DoubleHeroesPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Character/BlueHeroCharacter.h"
#include "Character/DoubleHeroesBaseCharacter.h"
#include "Character/DoubleHeroesCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Items/EquipmentManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/PackageUserWidget.h"
#include "UI/HUD/DoubleHeroesHUD.h"
#include "UI/HUD/WheelHUD.h"
#include "UI/Widget/DamageTextComponent.h"
#include "UI/WidgetController/DoubleHeroesSystemsWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

void ADoubleHeroesPlayerController::Server_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	if (ADoubleHeroesCharacter* ControlledCharacter = Cast<ADoubleHeroesCharacter>(GetPawn()))
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}



ADoubleHeroesPlayerController::ADoubleHeroesPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentManagerComponent>("EquipmentComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);
}

UInventoryComponent* ADoubleHeroesPlayerController::GetInventoryComponent_Implementation()
{
	return InventoryComponent;
}

void ADoubleHeroesPlayerController::SetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag, int32 AbilityLevel)
{
	if (IsValid(DHAbilitySystemComponent))
	{
		DHAbilitySystemComponent->SetDynamicProjectile(ProjectileTag);
	}
}

UAbilitySystemComponent* ADoubleHeroesPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
}

UInventoryWidgetController* ADoubleHeroesPlayerController::GetInventoryWidgetController()
{
	if (!IsValid(InventoryWidgetController))
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetOwningActor(this);
		InventoryWidgetController->BindCallbacksToDependencies();
	}

	return InventoryWidgetController;
}

void ADoubleHeroesPlayerController::CreateInventoryWidget()
{
	
	if(UUserWidget* Widget = CreateWidget<UDoubleHeroesSystemsWidget>(this, InventoryWidgetClass))
	{
		InventoryWidget = Cast<UDoubleHeroesSystemsWidget>(Widget);
		InventoryWidget->SetWidgetController(GetInventoryWidgetController());
		InventoryWidgetController->BroadcastInitialValues();
		InventoryWidget->AddToViewport();
	}
}

void ADoubleHeroesPlayerController::CreateWidgetController()
{
}

void ADoubleHeroesPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// CursorTrace();
	// AutoRun();
	if(bKeyW == false && bKeyS == false && bKeyA == false && bKeyD == false)
	{
		Input_StopRun();
	}
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

// void ADoubleHeroesPlayerController::AutoRun()
// {
// 	if (!bAutoRunning) return;
// 	if (ControlledPawn)
// 	{
// 		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
// 			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
// 		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
// 			LocationOnSpline, ESplineCoordinateSpace::World);
// 		ControlledPawn->AddMovementInput(Direction);
//
// 		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
// 		if (DistanceToDestination <= AutoRunAcceptanceRadius)
// 		{
// 			bAutoRunning = false;
// 		}
// 	}
// }

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

// void ADoubleHeroesPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
// {
// 	if (!InputTag.MatchesTagExact(FDoubleHeroesGameplayTags::Get().InputTag_LMB))
// 	{
// 		if (GetASC())
// 		{
// 			GetASC()->AbilityInputTagReleased(InputTag);
// 		}
// 		return;
// 	}
//
// 	if (GetASC())
// 	{
// 		GetASC()->AbilityInputTagReleased(InputTag);
// 	}
//
// 	if (!bTargeting || !bShiftKeyDown)
// 	{
// 		const APawn* ControlledPawn = GetPawn();
// 		if (FollowTime <= ShortPressThreshold && ControlledPawn)
// 		{
// 			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
// 				this, ControlledPawn->GetActorLocation(), CachedDestination))
// 			{
// 				Spline->ClearSplinePoints();
// 				for (const FVector& PointLoc : NavPath->PathPoints)
// 				{
// 					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
// 					// DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
// 				}
// 				// 防止点击到NavMeshBoundsVolume以外的地方时因为数组越界崩溃
// 				if (NavPath->PathPoints.Num() > 0)
// 				{
// 					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
// 					bAutoRunning = true;
// 				}
// 				else
// 				{
// 					// 处理空路径的情况
// 					UE_LOG(LogTemp, Warning, TEXT("No path points found"));
// 				}
// 			}
// 		}
// 		FollowTime = 0.f;
// 		bTargeting = false;
// 	}
// }

// void ADoubleHeroesPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
// {
// 	if (!InputTag.MatchesTagExact(FDoubleHeroesGameplayTags::Get().InputTag_LMB))
// 	{
// 		if (GetASC())
// 		{
// 			GetASC()->AbilityInputTagHeld(InputTag);
// 		}
// 		return;
// 	}
//
// 	if (bTargeting || bShiftKeyDown)
// 	{
// 		if (GetASC())
// 		{
// 			GetASC()->AbilityInputTagHeld(InputTag);
// 		}
// 	}
// 	else
// 	{
// 		FollowTime += GetWorld()->GetDeltaSeconds();
//
// 		if (CursorHit.bBlockingHit)
// 		{
// 			CachedDestination = CursorHit.ImpactPoint;
// 		}
//
// 		if (APawn* ControlledPawn = GetPawn())
// 		{
// 			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
// 			ControlledPawn->AddMovementInput(WorldDirection);
// 		}
// 	}
// }

UDHAbilitySystemComponent* ADoubleHeroesPlayerController::GetASC()
{
	//一
	if (DHAbilitySystemComponent == nullptr)
	{
		DHAbilitySystemComponent = Cast<UDHAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	//GAS
	// ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>();
	// check(DoubleHeroesPlayerState);
	// DHAbilitySystemComponent = DoubleHeroesPlayerState->GetDHAbilitySystemComponent();
	//GAS
	return DHAbilitySystemComponent;
}

void ADoubleHeroesPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// check(DoubleHeroesContext);

	// UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
	// 	GetLocalPlayer());
	// if (Subsystem)
	// {
	// 	Subsystem->AddMappingContext(DoubleHeroesContext, 0);
	// }

	// bShowMouseCursor = true;
	// DefaultMouseCursor = EMouseCursor::Default;

	// FInputModeGameAndUI InputModeData;
	//鼠标不锁定到窗口
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//不隐藏光标
	// InputModeData.SetHideCursorDuringCapture(false);
	// SetInputMode(InputModeData);
	ControlledPawn = GetPawn<APawn>();
	BaseCharacter = Cast<ADoubleHeroesBaseCharacter>(ControlledPawn);
	BindCallbacksToDependencies();
	if (BaseCharacter && HasAuthority())
	{
		BaseCharacter->SetReplicates(true);
		BaseCharacter->SetReplicateMovement(true);
	}
	DHAbilitySystemComponent = GetASC();
	
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}
}


// void ADoubleHeroesPlayerController::SetupInputComponent()
// {
// 	Super::SetupInputComponent();
//
// 	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(InputComponent);
// 	DoubleHeroesInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
// 	                                       &ADoubleHeroesPlayerController::Move);
// 	DoubleHeroesInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this,
// 	&ADoubleHeroesPlayerController::Run);
// 	DoubleHeroesInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this,
// 	&ADoubleHeroesPlayerController::Walk);
// 	
// 	DoubleHeroesInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
// 	                                       &ADoubleHeroesPlayerController::Look);
// 	DoubleHeroesInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this,
// 	&ADoubleHeroesPlayerController::ShiftPressed);
// 	DoubleHeroesInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this,
// 	&ADoubleHeroesPlayerController::ShiftReleased);
// }

void ADoubleHeroesPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(InputComponent);
	// DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);

	
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveForward, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveForward, ETriggerEvent::Started, this, &ThisClass::Input_PressW);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveForward, ETriggerEvent::Completed, this, &ThisClass::Input_ReleaseW);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveLeft, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveLeft, ETriggerEvent::Started, this, &ThisClass::Input_PressA);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveLeft, ETriggerEvent::Completed, this, &ThisClass::Input_ReleaseA);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveBackward, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveBackward, ETriggerEvent::Started, this, &ThisClass::Input_PressS);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveBackward, ETriggerEvent::Completed, this, &ThisClass::Input_ReleaseS);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveRight, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveRight, ETriggerEvent::Started, this, &ThisClass::Input_PressD);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_MoveRight, ETriggerEvent::Completed, this, &ThisClass::Input_ReleaseD);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_TogglePackage, ETriggerEvent::Started, this, &ThisClass::Input_OpenPackage);
	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_TogglePackage, ETriggerEvent::Completed, this, &ThisClass::Input_ClosePackage);
	DoubleHeroesInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
	// DoubleHeroesInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputReleased, &ThisClass::Input_AbilityInputReleased);
	// DoubleHeroesInputComponent->BindAbilityActions(DoubleHeroesInputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputPressed);
}

void ADoubleHeroesPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	// find out which way is forward
	// const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	// const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);
	// if (ControlledPawn)
	// {
	// 	if (MovementVector.Y != 0.f)
	// 	{
	// 		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	// 		bIsRunning = false;
	// 		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	//
	// 	}
	// 		
	//
	// 	if (MovementVector.X != 0.f)
	// 	{
	// 		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
	// 		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	// 	}
	// }
	if (BaseCharacter)
	{
		BaseCharacter->Input_Move(InputActionValue);
	}
}

void ADoubleHeroesPlayerController::Input_PressW(const FInputActionValue& InputActionValue)
{
	PressedKey = EKeys::W;
	bKeyW = true;
	Input_SaveClick(InputActionValue);
}

void ADoubleHeroesPlayerController::Input_PressA(const FInputActionValue& InputActionValue)
{
	PressedKey = EKeys::A;
	bKeyA = true;
	Input_SaveClick(InputActionValue);
}

void ADoubleHeroesPlayerController::Input_PressS(const FInputActionValue& InputActionValue)
{
	PressedKey = EKeys::S;
	bKeyS = true;
	Input_SaveClick(InputActionValue);
}

void ADoubleHeroesPlayerController::Input_PressD(const FInputActionValue& InputActionValue)
{
	PressedKey = EKeys::D;
	bKeyD = true;
	Input_SaveClick(InputActionValue);
}

void ADoubleHeroesPlayerController::Input_ReleaseW(const FInputActionValue& InputActionValue)
{
	bKeyW = false;
}

void ADoubleHeroesPlayerController::Input_ReleaseA(const FInputActionValue& InputActionValue)
{
	bKeyA = false;
}

void ADoubleHeroesPlayerController::Input_ReleaseS(const FInputActionValue& InputActionValue)
{
	bKeyS = false;
}

void ADoubleHeroesPlayerController::Input_ReleaseD(const FInputActionValue& InputActionValue)
{
	bKeyD = false;
}

void ADoubleHeroesPlayerController::Input_SaveClick(const FInputActionValue& InputActionValue)
{
	// MovementVector = InputActionValue.Get<FVector2D>();
	CurrentTime = FPlatformTime::Seconds();
	if ((PressedKey == LastPressedKey) && (CurrentTime - LastTime < DoubleClickThreshold))
	{
		Input_StartRun();
	}
	LastTime = CurrentTime;
	LastPressedKey = PressedKey;
}

void ADoubleHeroesPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	if (BaseCharacter && (!bTogglePackage))
	{
		BaseCharacter->Input_Look(InputActionValue);
	}
}

void ADoubleHeroesPlayerController::Input_StartRun()
{
	if (BaseCharacter)
	{
		BaseCharacter->bIsRunning = true;
	}
}

void ADoubleHeroesPlayerController::Input_StopRun()
{
	if (BaseCharacter)
	{
		BaseCharacter->bIsRunning = false;
	}
}


void ADoubleHeroesPlayerController::Input_OpenPackage()
{
	if (!PackageUserWidget)
	{
		if(UUserWidget* Widget = CreateWidget<UPackageUserWidget>(this, PackageUserWidgetClass))
		{
			PackageUserWidget = Cast<UPackageUserWidget>(Widget);
			PackageUserWidget->SetWidgetController(GetInventoryWidgetController());
			InventoryWidgetController->BroadcastInitialValues();
		}
	}
	PackageUserWidget->OpenPackageUI();
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	bTogglePackage = true;
	
}

void ADoubleHeroesPlayerController::Input_ClosePackage()
{
	if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(GetHUD()))
	{
		DoubleHeroesHUD->ClosePackageUI();
		bTogglePackage = false;
	}
}

void ADoubleHeroesPlayerController::Input_AbilityInputPressed(const FGameplayTag InputTag)
{
	DHAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void ADoubleHeroesPlayerController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	DHAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}

UDHAbilitySystemComponent* ADoubleHeroesPlayerController::GetDHAbilitySystemComponent()
{
	if (!IsValid(DHAbilitySystemComponent))
	{
		if(const ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>())
		{
			DHAbilitySystemComponent = DoubleHeroesPlayerState->GetDHAbilitySystemComponent();
		}
	}

	return DHAbilitySystemComponent;
}

void ADoubleHeroesPlayerController::BindCallbacksToDependencies()
{
	if (IsValid(InventoryComponent) && IsValid(EquipmentComponent))
	{
		InventoryComponent->EquipmentItemDelegate.AddLambda(
			[this] (const TSubclassOf<UEquipmentDefinition>& EquipmentDefinition, const FEquipmentEffectPackage& EffectPackage)
			{
				if (IsValid(EquipmentComponent))
				{
					EquipmentComponent->EquipItem(EquipmentDefinition, EffectPackage);
				}
			});

		EquipmentComponent->EquipmentList.UnEquippedEntryDelegate.AddLambda(
			[this] (const FDoubleHeroesEquipmentEntry& UnEquippedEntry)
			{
				if (IsValid(InventoryComponent))
				{
					InventoryComponent->AddUnEquippedItemEntry(UnEquippedEntry.EntryTag, UnEquippedEntry.EffectPackage);
				}
			});
	}
}

void ADoubleHeroesPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoubleHeroesPlayerController, InventoryComponent);
}

void ADoubleHeroesPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (IsValid(DHAbilitySystemComponent))
	{
		DHAbilitySystemComponent->OnAbilityInputPressed(InputTag);
	}
}
void ADoubleHeroesPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (IsValid(DHAbilitySystemComponent))
	{
		DHAbilitySystemComponent->OnAbilityInputReleased(InputTag);
	}
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


