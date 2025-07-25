// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "GameFramework/Character.h"
#include "Interaction/InteractableTarget.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(false);
	// ...
}

void UInteractionComponent::TryInteract()
{
}

void UInteractionComponent::OnInteractTargetChanged(AActor* NewTarget)
{
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractionComponent::CheckForInteractable()
{
	// TArray<AActor*> OverlappingActors;
	// GetOwner()->GetOverlappingActors(OverlappingActors);
	//
	// AActor* ClosestInteractable = nullptr;
	// float MinDistance = FLT_MAX;
	//
	// for (AActor* Actor : OverlappingActors)
	// {
	// 	// 检查Actor是否为可交互类型
	// 	if (Actor->Implements<UInteractableInterface>())
	// 	{
	// 		float Distance = (Actor->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	// 		if (Distance < MinDistance)
	// 		{
	// 			MinDistance = Distance;
	// 			ClosestInteractable = Actor;
	// 		}
	// 	}
	// }
	//
	// if (ClosestInteractable != CurrentInteractableTarget)
	// {
	// 	OnInteractTargetChanged(ClosestInteractable);
	// 	CurrentInteractableTarget = ClosestInteractable;
	// }
}

void UInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (!OtherActor || OtherActor == GetOwner()) return;
	//
	// if (OtherActor->Implements<IInteractableTarget>())
	// {
	// 	CurrentInteractables.AddUnique(OtherActor);
	// }
	//
	// TArray<TScriptInterface<IInteractableTarget>> InteractableTargets;
	//
	// for (TScriptInterface<IInteractableTarget>& InteractiveTarget : InteractableTargets)
	// {
	// 	FInteractionOptionBuilder InteractionBuilder(InteractiveTarget, Options);
	// 	InteractiveTarget->GatherInteractionOptions(InteractionQuery, InteractionBuilder);
	// }
}
