// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleHeroes/Public/Item/PressurePlate.h"

#include "DoubleHeroes/Public/Item/Transporter.h"

// Sets default values
APressurePlate::APressurePlate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//打开参与者的复制
	bReplicates = true;
	SetReplicateMovement(true);

	Activated = false;

	//创建根物体
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	//TriggerMesh创建后附加到根物体
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComponent);
	TriggerMesh->SetIsReplicated(true);

	//加载碰撞体模型
	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	//如果加载成功把模型作为碰撞体加入到TriggerMesh
	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		//缩放圆柱让它变得很扁，更贴近踏板
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	//创建Mesh存放踏板模型
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	//加载踏板模型
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("/Game/Stylized_Egypt/Meshes/building/SM_building_part_08.SM_building_part_08"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 0.5f));
		//设置为7.2而不是10是为了让它们不重叠，让模型在碰撞体之内
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 7.2f));
	}

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.25f;
	Transporter->OwnerIsTriggerActor = true;
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	//碰撞体不需要被看到
	TriggerMesh->SetVisibility(false);

	//WorldStatic object that overlaps all actors by default. All new custom channels will use its own default response.
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));

	FVector Point1 = GetActorLocation();
	FVector Point2 = Point1 + FVector(0.0f, 0.0f, -10.0f);
	Transporter->SetPoints(Point1, Point2);
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		AActor* TriggerActor = nullptr;
		//获取物体重叠的数组存到括号中的OverlappingActors变量中
		TriggerMesh->GetOverlappingActors(OverlappingActors);

		//查找碰撞的物体
		for (int ActorIdx = 0; ActorIdx < OverlappingActors.Num(); ++ActorIdx)
		{
			AActor* A = OverlappingActors[ActorIdx];
			//如果有这个标签
			if (A->ActorHasTag("TriggerActor"))
			{
				TriggerActor = A;
				break;
			}

			// FString Msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
			// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, Msg);
		}

		//如果非空
		if (TriggerActor)
		{
			//没激活就让它激活
			if (!Activated)
			{
				Activated = true;
				// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Activated"));
				OnActivated.Broadcast();
			}
		}
		//否则如果是空的
		else
		{
			//激活了就设置为没激活
			if (Activated)
			{
				Activated = false;
				// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Deactivated"));
				OnDeactivated.Broadcast();
			}
		}
	} //9. Transporter Constructor_zh_en
}
