// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlueHeroCharacter.h"

#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/HeroCombatComponent.h"
#include "Components/InputComponent.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "DoubleHeroesComponent/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"


// Sets default values
ABlueHeroCharacter::ABlueHeroCharacter()
{
	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	//Combat->SetIsReplicated(true); // 如果需要网络复制

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroCombatComponent->SetIsReplicated(true); // 如果需要网络复制
}

void ABlueHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME_CONDITION(ABlueHeroCharacter, OverlappingWeapon, COND_OwnerOnly); //COND_OwnerOnly只在网络的所有者客户端上复制该变量
}

// Called when the game starts or when spawned
void ABlueHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	// if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
	// 		UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		Subsystem->AddMappingContext(SlashContext, 0);
	// 	}
	// }
}

// Called every frame
void ABlueHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingWeapon)
	{
	}
}

/*void ABlueHeroCharacter::MoveForward(float Value)
{
	if (bDisableGameplay) return;
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}

	if(ActionState != EActionState::EAS_Unoccupied) return;
}*/

// void ABlueHeroCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
// {
// }

void ABlueHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(DHAbilitySystemComponent);
		}
	}
}

// void ABlueHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
// {
// 	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
// 	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
// 	check(Subsystem);
// 	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
// 	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(PlayerInputComponent);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Run, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_TogglePackage, ETriggerEvent::Triggered, this, &ThisClass::Input_TogglePackage);
// 	DoubleHeroesInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputPressed);
//
// }

// void ABlueHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
// {
// 	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
// 	{
// 		//Jumping
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
// 		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Punch);
// 		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Dodge);
// 		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
// 										   &ABlueHeroCharacter::Interact);
// 		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::CrouchPressed);
// 	}
// }

// void ABlueHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
// {
// 	// find out which way is forward
// 	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
// 	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
// 	if (MovementVector.Y != 0.f)
// 	{
// 		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
// 		AddMovementInput(ForwardDirection, MovementVector.Y);
// 	}
//
// 	if (MovementVector.X != 0.f)
// 	{
// 		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
// 		AddMovementInput(RightDirection, MovementVector.X);
// 	}
// }

// void ABlueHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
// {
// 	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
// 	if (LookAxisVector.X != 0.f)
// 	{
// 		AddControllerYawInput(LookAxisVector.X);
// 	}
// 	if (LookAxisVector.Y != 0.f)
// 	{
// 		AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }



// void ABlueHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
// {
// 	DHAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
// }

void ABlueHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	DHAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void ABlueHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// if (Combat)
	// {
	// 	Combat->Character = this;
	// }
}

// void ABlueHeroCharacter::SetOverlappingWeapon(AWeapon* Weapon)
// {
// 	if (OverlappingWeapon)
// 	{
// 		// OverlappingWeapon->ShowPickupWidget(false);
// 	}
// 	OverlappingWeapon = Weapon;
// 	if (IsLocallyControlled())
// 	{
// 		if (OverlappingWeapon)
// 		{
// 			// OverlappingWeapon->ShowPickupWidget(true);
// 		}
// 	}
// }

bool ABlueHeroCharacter::IsWeaponEquipped()
{
	return (HeroCombatComponent && HeroCombatComponent->EquippedWeapon);
}

void ABlueHeroCharacter::Punch()
{
}

void ABlueHeroCharacter::Dodge()
{
}

void ABlueHeroCharacter::CrouchPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void ABlueHeroCharacter::Interact()
{
	// if (Combat)
	// {
	// 	if (HasAuthority())
	// 	{
	// 		Combat->EquipWeapon(OverlappingWeapon);
	// 	}
	// 	else
	// 	{
	// 		ServerEquipButtonPressed();
	// 	}
	// }

	if (HeroCombatComponent)
	{
		if (HasAuthority())
		{
			HeroCombatComponent->EquipWeapon(OverlappingWeapon);
		}
	}
}

void ABlueHeroCharacter::ServerEquipButtonPressed_Implementation()
{
	if (HeroCombatComponent)
	{
		HeroCombatComponent->EquipWeapon(OverlappingWeapon);
	}
}
