﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WinArea.h"

#include "Character/CoopAdventureCharacter.h"


// Sets default values
AWinArea::AWinArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);

	WinCondition = false;
}

// Called when the game starts or when spawned
void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		if(!WinCondition)
		{
			TArray<AActor*> OverlapActors;
			WinAreaBox->GetOverlappingActors(OverlapActors, ACoopAdventureCharacter::StaticClass());

			//如果两个角色在框内
			WinCondition = (OverlapActors.Num() == 2);
			if (WinCondition)
			{
				UE_LOG(LogTemp, Display, TEXT("WIN"));

				MulticastRPCWin();
			}
		}
	}
}

void AWinArea::MulticastRPCWin_Implementation()
{
	if(HasAuthority())
	{
		OnWinCondition.Broadcast();
	}
}

