// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlueHeroCharacter.h"

#include "DoubleHeroesGameplayTags.h"
#include "LoadScreenSaveGame.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/DHCharacterMovementComponent.h"
#include "Components/HeroCombatComponent.h"
#include "Components/InputComponent.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "Game/DoubleHeroesGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "Weapon/Weapon.h"


// Sets default values
// ABlueHeroCharacter::ABlueHeroCharacter()
//GAS
ABlueHeroCharacter::ABlueHeroCharacter(const FObjectInitializer& ObjectInitializer)
:  Super(ObjectInitializer.SetDefaultSubobjectClass<UDHCharacterMovementComponent>(CharacterMovementComponentName))
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
	

	// 不要在构造函数中设置这些指针，而是在PostInitializeComponents中设置
	// 因为这时对象还没有完全构造完成
	
}

void ABlueHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlueHeroCharacter, OverlappingItem, COND_OwnerOnly);

	// DOREPLIFETIME_CONDITION(ABlueHeroCharacter, OverlappingItem, COND_OwnerOnly); //COND_OwnerOnly只在网络的所有者客户端上复制该变量
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

void ABlueHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// if (!CharacterStartUpData.IsNull())
	// {
	// 	if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
	// 	{
	// 		LoadedData->GiveToAbilitySystemComponent(DHAbilitySystemComponent);
	// 	}
	// }
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
	if (HeroCombatComponent)
	{
		HeroCombatComponent->SetIsReplicated(true); // 如果需要网络复制
		HeroCombatComponent->Character = this;
	}

	
}

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

void ABlueHeroCharacter::LoadProgress()
{
	ADoubleHeroesGameModeBase* DoubleHeroesGameMode = Cast<ADoubleHeroesGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (DoubleHeroesGameMode)
	{
		ULoadScreenSaveGame* SaveData = DoubleHeroesGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if (UDHAbilitySystemComponent* AuraASC = Cast<UDHAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			
			if (ADoubleHeroesPlayerState* DoubleHeroesPlayerState = Cast<ADoubleHeroesPlayerState>(GetPlayerState()))
			{
				
				DoubleHeroesPlayerState->SetSkillPoints(SaveData->SkillPoints);
			}
			
			UDoubleHeroesAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void ABlueHeroCharacter::Interact()
{
	// if (Combat)
	// {
	// 	if (HasAuthority())
	// 	{
	// 		Combat->EquipWeapon(OverlappingItem);
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
			// HeroCombatComponent->EquipWeapon(OverlappingItem);
		}
	}
}

void ABlueHeroCharacter::ServerEquipButtonPressed_Implementation()
{
	if (HeroCombatComponent)
	{
		// HeroCombatComponent->EquipWeapon(OverlappingItem);
	}
}
