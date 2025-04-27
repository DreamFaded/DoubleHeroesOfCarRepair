// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitCrouchClearance.h"

#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_WaitCrouchClearance::UAbilityTask_WaitCrouchClearance()
{
	bTickingTask = true;
}

void UAbilityTask_WaitCrouchClearance::TickTask(float DeltaTime)
{
	if (!IsValid(AvatarActor))
	{
		EndTask();
	}
	Super::TickTask(DeltaTime);

	FVector TraceEnd = AvatarActor->GetActorLocation() + FVector(0.f, 0.f, TargetHeightOffset);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(AvatarActor);

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		AvatarActor->GetActorLocation(),
		TraceEnd,
		SphereTraceRadius,
		TraceTypeQuery1,
		false,
		IgnoreActors,
		bShowDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHit,
		true);

	// If there is no blocking collision directly above us within this redius, then we have clearance to stop crouching.
	if (!TraceHit.bBlockingHit)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			HasClearance.Broadcast();
			EndTask();
		}
	}
}

UAbilityTask_WaitCrouchClearance* UAbilityTask_WaitCrouchClearance::WaitCrouchClearance(UGameplayAbility* OwningAbility,
	float TargetHeightOffset, float SphereTraceRadius, bool bShowDebug)
{
	UAbilityTask_WaitCrouchClearance* MyObj = NewAbilityTask<UAbilityTask_WaitCrouchClearance>(OwningAbility);
	MyObj->TargetHeightOffset = TargetHeightOffset;
	MyObj->SphereTraceRadius = SphereTraceRadius;
	MyObj->bShowDebug = bShowDebug;
	MyObj->AvatarActor = MyObj->GetAvatarActor();

	return MyObj;
}

