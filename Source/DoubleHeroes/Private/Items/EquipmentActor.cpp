// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EquipmentActor.h"


// Sets default values
AEquipmentActor::AEquipmentActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);

	RootScene = CreateDefaultSubobject<USceneComponent>("RootScene");
	SetRootComponent(RootScene);

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>("EquipmentMesh");
	EquipmentMesh->SetupAttachment(GetRootComponent());
	EquipmentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

