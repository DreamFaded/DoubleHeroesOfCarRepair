// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/BlueHeroCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Item/SceneItemActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	//对所有碰撞通道设置为阻挡（ECR_Block）
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	//对角色通道（ECC_Pawn）设置为忽略（ECR_Ignore）
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//禁用碰撞
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);*/
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// if (GetLocalRole() == ROLE_Authority)
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //只启用查询模式
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //当碰撞体与 “Pawn” 类型的对象发生重叠时，会产生重叠事件
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		// AWeapon 类中的一个成员函数 OnSphereOverlap，这个函数将在碰撞体开始重叠时被调用
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap); // 事件委托，当碰撞体结束与其他物体重叠时会被触发
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	ADoubleHeroesBaseCharacter* BaseCharacter = Cast<ADoubleHeroesBaseCharacter>(OtherActor);

	// if (BaseCharacter)
	// {
		// if (ASceneItemActor* SceneItemActor = Cast<ASceneItemActor>(OtherActor))
		// {
		// 	if(PackageComponent)
		// 	{
		// 		PackageComponent->AddNearItem(SceneItemActor);
		// 	}
		// }
	// 	BaseCharacter->SetOverlappingItem(this);
	// }
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADoubleHeroesBaseCharacter* BaseCharacter = Cast<ADoubleHeroesBaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		// if (ASceneItemActor* SceneItemActor = Cast<ASceneItemActor>(OtherActor))
		// {
		// 	if(PackageComponent)
		// 	{
		// 		PackageComponent->RemoveNearItem(SceneItemActor);
		// 	}
		// }
		BaseCharacter->SetOverlappingItem(nullptr);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		// ShowPickupWidget(false);
		break;
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}
