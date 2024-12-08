// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkinComponent.h"


// Sets default values for this component's properties
USkinComponent::USkinComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USkinComponent::OnPutOnItem(ESkinPartType PartType, int32 ItemID)
{
	if (FItemBase* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(ItemID))
	{
		if (ItemBase->Type != EItemType::EPT_SkinPart)
		{
			return;
		}
		FSkinPart* SkinPart = static_cast<FSkinPart*>(ItemBase);
		if (SkinPart->PartType == ESkinPartType::ESPT_Clothes)
		{
			
		}
		else
		{
			if (!SMCMap.Contains(PartType))
			{
				UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner());
				StaticMeshComponent->RegisterComponentWithWorld(GetWorld());
				
				// if(IISkinInterface* Interface = Cast<>)
			}
		}
	}
}

void USkinComponent::OnTakeOffItem(ESkinPartType PartType, int32 ItemID)
{
}


// Called when the game starts
void USkinComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


FName USkinComponent::GetSocketName(ESkinPartType PartType)
{
	FName SocketName = TEXT("None");
	switch (PartType)
	{
	case ESkinPartType::ESPT_Beard:
	case ESkinPartType::ESPT_Cap:
	case ESkinPartType::ESPT_Glasses:
	case ESkinPartType::ESPT_Hair:
	case ESkinPartType::ESPT_Headset:
	case ESkinPartType::ESPT_Helmet:
	case ESkinPartType::ESPT_Masker:
		SocketName = TEXT("HeadSocket");
		break;
	}
	return SocketName;
}

// Called every frame
void USkinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

