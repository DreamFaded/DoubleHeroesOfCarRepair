// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesBaseCharacter.h"

#include "InputActionValue.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Components/SkinComponent.h"
#include "Data/CharacterClassInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Libraries/DHAbilitySystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/DoubleHeroesPlayerState.h"


// Sets default values
ADoubleHeroesBaseCharacter::ADoubleHeroesBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PackageComponent = CreateDefaultSubobject<UPackageComponent>(TEXT("PackageComponent"));
	SkinComponent = CreateDefaultSubobject<USkinComponent>(TEXT("SkinComponent"));

	GetMesh()->bReceivesDecals = false;

	DHAbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("DoubleHeroesAbilitySystemComponent"));

	DoubleHeroesAttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("DoubleHeroesAttributeSet"));

	
}

void ADoubleHeroesBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	if (PackageComponent)
	{
		PackageComponent->OnPutOnItem.AddUObject(SkinComponent, &USkinComponent::OnPutOnItem);
		PackageComponent->OnTakeOffItem.AddUObject(SkinComponent, &USkinComponent::OnTakeOffItem);
	}
}

USkeletalMeshComponent* ADoubleHeroesBaseCharacter::GetSkeletalMeshComponent()
{
	return GetMesh();
}

AWeapon* ADoubleHeroesBaseCharacter::GetHoldWeapon() const
{
	if (PackageComponent)
	{
		return PackageComponent->GetHoldWeapon();
	}
	return nullptr;
}


UAbilitySystemComponent* ADoubleHeroesBaseCharacter::GetAbilitySystemComponent() const
{
	return GetDHAbilitySystemComponent();
}

void ADoubleHeroesBaseCharacter::Server_SetRunning_Implementation(bool bNewRunning)
{
	bIsRunning = bNewRunning;
}

void ADoubleHeroesBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADoubleHeroesBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ADoubleHeroesBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, OverlappingWeapon, COND_OwnerOnly); //COND_OwnerOnly只在网络的所有者客户端上复制该变量
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, DHAbilitySystemComponent);
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, bIsRunning);
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, bEquipped);
}


void ADoubleHeroesBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();

		ensureMsgf(!CharacterStartUpData.IsNull(),TEXT("Forgot to assign start up to %s"), *GetName());
	}
}

void ADoubleHeroesBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void ADoubleHeroesBaseCharacter::InitAbilityActorInfo()
{
	if (ADoubleHeroesPlayerState* DoubleHeroesPlayerState = GetPlayerState<ADoubleHeroesPlayerState>())
	{
		DHAbilitySystemComponent = DoubleHeroesPlayerState->GetDHAbilitySystemComponent();
		DoubleHeroesAttributes = DoubleHeroesPlayerState->GetDoubleHeroesAttributes();

		if (IsValid(DHAbilitySystemComponent))
		{
			DHAbilitySystemComponent->InitAbilityActorInfo(DoubleHeroesPlayerState, this);
		}
	}
}

void ADoubleHeroesBaseCharacter::InitClassDefaults()
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
}

void ADoubleHeroesBaseCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
}

bool ADoubleHeroesBaseCharacter::IsRunning()
{
	return bIsRunning;
}

void ADoubleHeroesBaseCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		// OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			// OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ADoubleHeroesBaseCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// find out which way is forward
	MovementVector = InputActionValue.Get<FVector2D>();
	MovementRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	
	}
	else if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADoubleHeroesBaseCharacter::Input_Look(const FInputActionValue& InputActionValue)
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

void ADoubleHeroesBaseCharacter::Input_StartRun()
{
	if (bIsRunning) return; // 避免重复设置
	//如果在客户端，需要上服务器执行再用Rep函数调用
	if (!HasAuthority())
	{
		Server_SetRunning(true);
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ADoubleHeroesBaseCharacter::Input_StopRun()
{
	if (!bIsRunning) return;
	//如果在客户端，需要上服务器执行
	if (!HasAuthority())
	{
		Server_SetRunning(false);
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ADoubleHeroesBaseCharacter::OnRep_Run()
{
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

