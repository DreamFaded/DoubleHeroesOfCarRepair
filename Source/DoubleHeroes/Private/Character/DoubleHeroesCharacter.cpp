// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacter.h"

#include "AbilitySystemComponent.h"
#include "DoubleHeroesGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "Camera/CameraComponent.h"
#include "Components/DHCharacterMovementComponent.h"
#include "Data/CharacterClassInfo.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/DoubleHeroesInputComponent.h"
#include "Libraries/DHAbilitySystemLibrary.h"
#include "Player/DoubleHeroesPlayerController.h"
#include "Player/DoubleHeroesPlayerState.h"
#include "UI/HUD/DoubleHeroesHUD.h"


// Sets default values
ADoubleHeroesCharacter::ADoubleHeroesCharacter(const FObjectInitializer& ObjectInitializer)
	:  Super(ObjectInitializer.SetDefaultSubobjectClass<UDHCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	DynamicProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>("ProjectileSpawnPoint");
	DynamicProjectileSpawnPoint->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ADoubleHeroesCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	if (HasAuthority())
	{
		InitAbilityActorInfo();
	}
	// if (!CharacterStartUpData.IsNull())
	// {
	// 	if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
	// 	{
	// 		LoadedData->GiveToAbilitySystemComponent(DHAbilitySystemComponent);
	// 	}
	// }
	// AddCharacterAbilities();
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

USceneComponent* ADoubleHeroesCharacter::GetDynamicSpawnPoint_Implementation()
{
	// return IDoubleHeroesAbilitySystemInterface::GetDynamicSpawnPoint_Implementation();
	return DynamicProjectileSpawnPoint;
}

// void ADoubleHeroesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
// 	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
// 	check(Subsystem);
// 	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);
// 	UDoubleHeroesInputComponent* DoubleHeroesInputComponent = CastChecked<UDoubleHeroesInputComponent>(PlayerInputComponent);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
// 	DoubleHeroesInputComponent->BindNativeInputAction(InputConfigDataAsset, DoubleHeroesGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
//
// }

void ADoubleHeroesCharacter::InitAbilityActorInfo()
{
	ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>();
	check(DoubleHeroesPlayerState);
	DoubleHeroesPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DoubleHeroesPlayerState, this);
	Cast<UDHAbilitySystemComponent>(DoubleHeroesPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	DHAbilitySystemComponent = DoubleHeroesPlayerState->GetDHAbilitySystemComponent();
	DoubleHeroesAttributes = DoubleHeroesPlayerState->GetDoubleHeroesAttributes();


	//HUD
	if (ADoubleHeroesPlayerController* DoubleHeroesPlayerController = Cast<ADoubleHeroesPlayerController>(GetController()))
	{
		if(ADoubleHeroesHUD* DoubleHeroesHUD = Cast<ADoubleHeroesHUD>(DoubleHeroesPlayerController->GetHUD()))
		{
			DoubleHeroesHUD->InitOverlay(DoubleHeroesPlayerController, DoubleHeroesPlayerState, DHAbilitySystemComponent, DoubleHeroesAttributes);
		}
	}
	// if(ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>())
	// {
	// 	DHAbilitySystemComponent = DoubleHeroesPlayerState->GetDHAbilitySystemComponent();
	// 	DoubleHeroesAttributes = DoubleHeroesPlayerState->GetDoubleHeroesAttributes();
	// }
	if (IsValid(DHAbilitySystemComponent))
	{
		DHAbilitySystemComponent->InitAbilityActorInfo(DoubleHeroesPlayerState, this);
		BindCallbacksToDependencies();
		if (HasAuthority())
		{
			InitClassDefaults();
		}

		if (UDoubleHeroesAnimInstance* DoubleHeroesAnimInstance = Cast<UDoubleHeroesAnimInstance>(
			GetMesh()->GetAnimInstance()))
		{
			DoubleHeroesAnimInstance->InitializeWithAbilitySystem(DHAbilitySystemComponent);
		}
	}
	InitializeDefaultAttributes();
}

// void ADoubleHeroesCharacter::Input_Move(const FInputActionValue& InputActionValue)
// {
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
//
// void ADoubleHeroesCharacter::Input_Look(const FInputActionValue& InputActionValue)
// {
// 	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
// 	if (LookAxisVector.X != 0.f)
// 	{
// 	AddControllerYawInput(LookAxisVector.X);
// 	}
// 	if (LookAxisVector.Y != 0.f)
// 	{
// 	AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }

void ADoubleHeroesCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{ 
		UE_LOG(LogTemp, Error, TEXT("No Character Tag Selected In This Chatacter %s"), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = UDHAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(DHAbilitySystemComponent))
			{
				DHAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				DHAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				DHAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Character Class Info Found For This Character %s"), *GetNameSafe(this));
	}
}

void ADoubleHeroesCharacter::BindCallbacksToDependencies()
{
	if (IsValid(DHAbilitySystemComponent) && IsValid(DoubleHeroesAttributes))
	{
		// GetGameplayAttributeValueChangeDelegate用于获取某个属性值变化时的委托
		DHAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			DoubleHeroesAttributes->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged(Data.NewValue, DoubleHeroesAttributes->GetMaxHealth());
		});

		DHAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			DoubleHeroesAttributes->GetEnduranceAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnEnduranceChanged(Data.NewValue, DoubleHeroesAttributes->GetMaxEndurance());
		});
		
	}
}

void ADoubleHeroesCharacter::BroadcastInitialValues()
{
	if (IsValid(DoubleHeroesAttributes))
	{
		OnHealthChanged(DoubleHeroesAttributes->GetHealth(), DoubleHeroesAttributes->GetMaxHealth());
		OnEnduranceChanged(DoubleHeroesAttributes->GetEndurance(), DoubleHeroesAttributes->GetMaxEndurance());
	}
}
