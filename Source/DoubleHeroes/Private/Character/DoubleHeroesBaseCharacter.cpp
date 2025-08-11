// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesBaseCharacter.h"

#include "InputActionValue.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "AnimInstances/DoubleHeroesAnimInstance.h"
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

AItemBase* ADoubleHeroesBaseCharacter::GetHoldItem() const
{
	// if (PackageComponent)
	// {
	// 	return PackageComponent->GetHoldItem();
	// }
	return nullptr;
}


UAbilitySystemComponent* ADoubleHeroesBaseCharacter::GetAbilitySystemComponent() const
{
	return GetDHAbilitySystemComponent();
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

// void ADoubleHeroesBaseCharacter::Server_StartRunning_Implementation()
// {
// 	Input_StartRun();
// }
//
//
// void ADoubleHeroesBaseCharacter::Server_StopRunning_Implementation()
// {
// 	Input_StopRun();
// }

void ADoubleHeroesBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME_CONDITION(ThisClass, OverlappingItem, COND_OwnerOnly); //COND_OwnerOnly只在网络的所有者客户端上复制该变量
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, DHAbilitySystemComponent);
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, bIsRunning);
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, bEquipped);
	DOREPLIFETIME(ADoubleHeroesBaseCharacter, bIsStabilizing);
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
				DHAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartupAbilities);
				DHAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				DHAbilitySystemComponent->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
}

void ADoubleHeroesBaseCharacter::OnRep_OverlappingItem(AItemBase* LastItem)
{
	// if (OverlappingItem)
	// {
	// 	OverlappingItem->ShowPickupWidget(true);
	// }
	// if (LastItem)
	// {
	// 	LastItem->ShowPickupWidget(false);
	// }
}

bool ADoubleHeroesBaseCharacter::IsRunning()
{
	return bIsRunning;
}

void ADoubleHeroesBaseCharacter::SetOverlappingItem(AItemBase* Item)
{
	if (OverlappingItem)
	{
		// OverlappingItem->ShowPickupWidget(false);
	}
	OverlappingItem = Item;
	if (IsLocallyControlled())
	{
		if (OverlappingItem)
		{
			// OverlappingItem->ShowPickupWidget(true);
		}
	}
}

void ADoubleHeroesBaseCharacter::AddCharacterAbilities()
{
	UDHAbilitySystemComponent* AuraASC = CastChecked<UDHAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ADoubleHeroesBaseCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// if (bIsStabilizing)
	// {
	// 	return; // 如果正处于站稳阶段，直接返回，不处理移动输入
	// }
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

// void ADoubleHeroesBaseCharacter::Landed(const FHitResult& Hit)
// {
// 	Super::Landed(Hit);
//
// 	if (!HasAuthority()) return;
//
// 	// 开启站稳状态
// 	bIsStabilizing = true;
//
// 	// 设置站稳持续时间（比如 0.5 秒）
// 	GetWorld()->GetTimerManager().SetTimer(StabilizeTimerHandle, [this]()
// 	{
// 		bIsStabilizing = false;
// 	}, 0.3f, false);
//
// }

// void ADoubleHeroesBaseCharacter::Input_StartRun()
// {
	// if (bIsRunning) return; // 避免重复设置
	//如果在客户端，需要上服务器执行再用Rep函数调用
	// if (!HasAuthority())
	// {
	// 	Server_StartRunning();
	// 	return;
	// }
	// bIsRunning = true;
	//速度在DHCharacterMovementComponent中设置了
// }

// void ADoubleHeroesBaseCharacter::Input_StopRun()
// {
	// if (!bIsRunning) return;
	//如果在客户端，需要上服务器执行
	// if (!HasAuthority())
	// {
	// 	Server_StopRunning();
	// 	return;
	// }
	// bIsRunning = false;
	//速度在DHCharacterMovementComponent中设置了
// }


