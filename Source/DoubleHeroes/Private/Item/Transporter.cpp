// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Transporter.h"

#include "Item/CollectableKey.h"
#include "Item/PressurePlate.h"

// Sets default values for this component's properties
UTransporter::UTransporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;
	ArePointsSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();

	// ...
}

void UTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2))
	{
		return;
	}
	StartPoint = Point1;
	EndPoint = Point2;
	ArePointsSet = true;
}


RETURN_IF_EXIT_REQUESTED;
// Called when the game starts
void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	if(OwnerIsTriggerActor)
	{
		TriggerActors.Add(GetOwner());
	}

	for (AActor* TA : TriggerActors)
	{
		APressurePlate* PressurePlateActor = Cast<APressurePlate>(TA);
		if (PressurePlateActor)
		{
			PressurePlateActor->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
			PressurePlateActor->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}

		ACollectableKey* KeyActor = Cast<ACollectableKey>(TA);
		if (KeyActor)
		{
			KeyActor->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}
}

void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;

	FString Msg = FString::Printf(TEXT("Transporter Activated : %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Msg);
}

//代理
void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;

	FString Msg = FString::Printf(TEXT("Transporter Activated : %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, Msg);
}

// Called every frame
void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerActors.Num() > 0)
	{
		AllTriggerActorsTriggered = (ActivatedTriggerCount >= TriggerActors.Num());
		if (AllTriggerActorsTriggered)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("All TriggerActors Triggered"));
		}
	}

	AActor* MyOwner = GetOwner();
	if(MyOwner && MyOwner->HasAuthority() && ArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint)/MoveTime;
		// 确定目标位置，如果所有触发器都被触发，则目标位置为结束点，否则为起始点
		FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;
		// 如果当前位置不等于目标位置，则进行位置插值移动
		if(!CurrentLocation.Equals(TargetLocation))
		{
			// 根据时间插值和速度，计算从当前位置到目标位置的新位置
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}
