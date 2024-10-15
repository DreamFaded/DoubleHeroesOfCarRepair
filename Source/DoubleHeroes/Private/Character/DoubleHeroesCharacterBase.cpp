// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacterBase.h"


// Sets default values
ADoubleHeroesCharacterBase::ADoubleHeroesCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ADoubleHeroesCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ADoubleHeroesCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void ADoubleHeroesCharacterBase::InitAbilityActorInfo()
{
}
