// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "Components/BoxComponent.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Durability = 100.0f;
	MaxDurability = 100.0f;
	

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	//对所有碰撞通道设置为阻挡（ECR_Block）
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	//对角色通道（ECC_Pawn）设置为忽略（ECR_Ignore）
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//禁用碰撞
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaBox"));
	AreaBox->SetupAttachment(RootComponent);
	AreaBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AItemBase::UseItem()
{
}

void AItemBase::Destroyed()
{
	Super::Destroyed();
}

int64 AItemBase::GenerateID()
{
	int64 Timestamp = FDateTime::Now().ToUnixTimestamp() * 1000000;
	int64 RandomBits = FMath::RandRange(0, 0xFFFF);
	return (Timestamp << 16) | RandomBits;


	// int32 NewID = ++LastAssignedID;
	//
	// int32 SignatureIndex = 0;
	// while (SignatureIndex < 12)
	// {
	// 	if (FMath::RandRange(0, 100) < 85)
	// 	{
	// 		NewID |= (uint64)1 << FMath::RandRange(0, 63);
	// 	}
	// 	++SignatureIndex;
	// }
	// return NewID;
}

void AItemBase::SetItemState(EItemState State)
{
	ItemState = State;
	switch (ItemState)
	{
	case EItemState::EIS_Equipped:
		AreaBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AItemBase::OnRep_ItemState()
{
	switch (ItemState)
	{
	case EItemState::EIS_Equipped:
		// ShowPickupWidget(false);
			break;
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateID();
	
}

void AItemBase::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AItemBase::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

