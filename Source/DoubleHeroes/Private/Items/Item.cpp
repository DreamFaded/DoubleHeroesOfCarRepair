// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Character/DoubleHeroesCharacter.h"
#include "Components/BoxComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime);
}

void AItem::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}

	ADoubleHeroesCharacter* Character = Cast<ADoubleHeroesCharacter>(OtherActor);
	if (Character)
	{
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		ItemMesh->AttachToComponent(Character->GetMesh(), TransformRules, FName("RightHandSocket"));
	}
}

void AItem::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBoxOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AItem::OnBoxEndOverlap);
}
