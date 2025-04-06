// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DoubleHeroesItemInstance.h"

#include "Net/UnrealNetwork.h"

// #if UE_WITH_IRIS
// #include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
// #endif // UE_WITH_IRIS

#include "GameFramework/Character.h"
#include "Items/DoubleHeroesItemDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DoubleHeroesItemInstance)


UDoubleHeroesItemInstance::UDoubleHeroesItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UWorld* UDoubleHeroesItemInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UDoubleHeroesItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

#if UE_WITH_IRIS
void UDoubleHeroesItemInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;

	Super::RegisterReplicationFragments(Context, RegistrationFlags);

	// Build descriptors and allocate PropertyReplicationFragments for this object
	// FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS

APawn* UDoubleHeroesItemInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UDoubleHeroesItemInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UDoubleHeroesItemInstance::SpawnItemActors(const TArray<FDoubleHeroesItemActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FDoubleHeroesItemActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UDoubleHeroesItemInstance::DestroyItemActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UDoubleHeroesItemInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UDoubleHeroesItemInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UDoubleHeroesItemInstance::OnRep_Instigator()
{
}