// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DoubleHeroes/DoubleHeroes.h"


// Sets default values
ADoubleHeroesCharacterBase::ADoubleHeroesCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	/*Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
}

UAbilitySystemComponent* ADoubleHeroesCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ADoubleHeroesCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ADoubleHeroesCharacterBase::Die()
{
	// Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void ADoubleHeroesCharacterBase::MulticastHandleDeath_Implementation()
{
	// Weapon->SetSimulatePhysics(true);
	// Weapon->SetEnableGravity(true);
	// Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

// Called when the game starts or when spawned
void ADoubleHeroesCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// FVector ADoubleHeroesCharacterBase::GetCombatSocketLocation()
// {
	// check(Weapon);
	// return Weapon->GetSocketLocation(WeaponTipSocketName);
// }

void ADoubleHeroesCharacterBase::InitAbilityActorInfo()
{
}

void ADoubleHeroesCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADoubleHeroesCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	
}

void ADoubleHeroesCharacterBase::AddCharacterAbilities()
{
	UDHAbilitySystemComponent* DoubleHeroesASC = CastChecked<UDHAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	// DoubleHeroesASC->AddCharacterAbilities(StartupAbilities);
}

void ADoubleHeroesCharacterBase::Dissolve()
{
	/*if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}*/
}

/*
void ADoubleHeroesCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ADoubleHeroesCharacterBase::InitializeSecondaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultSecondaryAttributes);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultSecondaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
*/

