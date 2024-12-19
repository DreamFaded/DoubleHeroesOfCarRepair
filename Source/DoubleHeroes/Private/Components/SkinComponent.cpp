// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkinComponent.h"

#include "Interface/ISkinInterface.h"


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
	//如果道具数据存在
	if (FItemBase* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(ItemID))
	{
		//检测道具类型
		if (ItemBase->Type != EItemType::EPT_SkinPart)
		{
			return;
		}
		//转成Part
		FSkinPart* SkinPart = static_cast<FSkinPart*>(ItemBase);
		//如果是衣服
		if (SkinPart->PartType == ESkinPartType::ESPT_Clothes)
		{
			if (!SkeletalMeshComponent)
			{
				SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(GetOwner());
				SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
				if(IISkinInterface* Interface = Cast<IISkinInterface>(GetOwner()))
				{
					SkeletalMeshComponent->AttachToComponent(Interface->GetSkeletalMeshComponent(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					SkeletalMeshComponent->SetLeaderPoseComponent(Interface->GetSkeletalMeshComponent());
				}
			}
			SkeletalMeshComponent->SetSkeletalMesh(SkinPart->SkeletalMesh);
		}
		else
		{
			//检测组件是否存在
			if (!SMCMap.Contains(PartType))
			{
				UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner());
				StaticMeshComponent->RegisterComponentWithWorld(GetWorld());

				if (IISkinInterface* Interface = Cast<IISkinInterface>(GetOwner()))
				{
					StaticMeshComponent->AttachToComponent(Interface->GetSkeletalMeshComponent(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						GetSocketName(PartType));
				}
				SMCMap.Add(PartType, StaticMeshComponent);
			}
			SMCMap[PartType]->SetStaticMesh(SkinPart->StaticMesh);
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
		SocketName = TEXT("HeadSocket");//以上类型放此Socket
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

