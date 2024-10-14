// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesCharacter.h"


// Sets default values
ADoubleHeroesCharacter::ADoubleHeroesCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoubleHeroesCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoubleHeroesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADoubleHeroesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

