// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacterBase.h"


// Sets default values
ADoubleHeroesCharacterBase::ADoubleHeroesCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoubleHeroesCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoubleHeroesCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADoubleHeroesCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

