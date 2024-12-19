// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SceneItemActor.h"

#include "Subsystem/ItemSubsystem.h"


// Sets default values
ASceneItemActor::ASceneItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemID = -1;
}

void ASceneItemActor::SetID(int32 OutID, bool bInit)
{
	ID = OutID;
	if (bInit)
	{
		Init();
	}
}

void ASceneItemActor::SetItemID(int32 OutID)
{
	ItemID = OutID;
}

// Called when the game starts or when spawned
void ASceneItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASceneItemActor::Init()
{
	if (ItemID == -1)
	{
		return;
	}

	if(FItemBase* ItemBase = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>()->GetItemData(ItemID))
	{
		if (ItemBase->Type == EItemType::EPT_Weapon)
		{
			// if(!SMCMap)
		}
		else if (ItemBase->Type == EItemType::EPT_SkinPart)
		{
			FSkinPart* SkinPart = static_cast<FSkinPart*>(ItemBase);
			if (SkinPart->StaticMesh)
			{
				//动态组件创建
				if (!StaticMeshComponent)
				{
					StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
					StaticMeshComponent->RegisterComponentWithWorld(GetWorld());//注册到世界
					//设置依附父节点
					StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
				StaticMeshComponent->SetStaticMesh(SkinPart->StaticMesh);
			}
			else if (SkinPart->SkeletalMesh)
			{
				if (!SkeletalMeshComponent)
				{
					SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
					SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
					SkeletalMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					SkeletalMeshComponent->SetCollisionProfileName(TEXT("SceneItem"));
					SkeletalMeshComponent->SetGenerateOverlapEvents(true);
				}
				SkeletalMeshComponent->SetSkeletalMesh(SkinPart->SkeletalMesh);
			}
		}
		else
		{
			//武器的
			FWeaponInfo* WeaponInfo = static_cast<FWeaponInfo*>(ItemBase);
			if (!SkeletalMeshComponent)
			{
				SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
				SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
				SkeletalMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				SkeletalMeshComponent->SetCollisionProfileName(TEXT("SceneItem"));
				SkeletalMeshComponent->SetGenerateOverlapEvents(true);
			}
			SkeletalMeshComponent->SetSkeletalMesh(WeaponInfo->SkeletalMesh);
		}
	}
}

// Called every frame
void ASceneItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

