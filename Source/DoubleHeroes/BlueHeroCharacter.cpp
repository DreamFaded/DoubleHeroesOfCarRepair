// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueHeroCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "DoubleHeroesComponent/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
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

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	Combat->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ABlueHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}

}

// Called every frame
void ABlueHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingWeapon)
	{
		
	}
}

void ABlueHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlueHeroCharacter, OverlappingWeapon, COND_OwnerOnly);//COND_OwnerOnly只在网络的所有者客户端上复制该变量
}

void ABlueHeroCharacter::MoveForward(float Value)
{
	// if (bDisableGameplay) return;
	/*if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}*/

	// if(ActionState != EActionState::EAS_Unoccupied) return;
}

void ABlueHeroCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	
}

void ABlueHeroCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	/*FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, MovementVector.Y);
	const FVector Right = GetActorRightVector();
	AddMovementInput(Right, MovementVector.X);*/

	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABlueHeroCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//Moving
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Look);
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Punch);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Dodge);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABlueHeroCharacter::Interact);

	}

}

void ABlueHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Combat)
	{
		Combat->Character = this;
	}
	
}

void ABlueHeroCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	/*if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}*/
}

void ABlueHeroCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	/*if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}*/
	OverlappingWeapon = Weapon;
	/*if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}*/
}

void ABlueHeroCharacter::Punch()
{
}

void ABlueHeroCharacter::Dodge()
{
}

void ABlueHeroCharacter::Interact()
{
	if (Combat && HasAuthority())
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}
