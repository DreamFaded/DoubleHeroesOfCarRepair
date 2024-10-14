// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesEnemy.h"


// Sets default values
ADoubleHeroesEnemy::ADoubleHeroesEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoubleHeroesEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoubleHeroesEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADoubleHeroesEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

