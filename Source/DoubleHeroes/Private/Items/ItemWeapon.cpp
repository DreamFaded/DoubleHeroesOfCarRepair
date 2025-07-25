// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemWeapon.h"

#include "Character/DoubleHeroesBaseCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AItemWeapon::AItemWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AItemWeapon::UseItem()
{
	Super::UseItem();
}

// Called when the game starts or when spawned
void AItemWeapon::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //只启用查询模式
	AreaBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //当碰撞体与 “Pawn” 类型的对象发生重叠时，会产生重叠事件
	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &AItemWeapon::OnBoxOverlap);
	// AWeapon 类中的一个成员函数 OnItemOverlap，这个函数将在碰撞体开始重叠时被调用
	AreaBox->OnComponentEndOverlap.AddDynamic(this, &AItemWeapon::OnBoxEndOverlap); // 事件委托，当碰撞体结束与其他物体重叠时会被触发
	
}

void AItemWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Super::OnBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	ADoubleHeroesBaseCharacter* BaseCharacter = Cast<ADoubleHeroesBaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		// if (ASceneItemActor* SceneItemActor = Cast<ASceneItemActor>(OtherActor))
		// {
		// 	if(PackageComponent)
		// 	{
		// 		PackageComponent->AddNearItem(SceneItemActor);
		// 	}
		// }
		BaseCharacter->SetOverlappingItem(this);
	}
}

void AItemWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Super::OnBoxEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
	ADoubleHeroesBaseCharacter* BaseCharacter = Cast<ADoubleHeroesBaseCharacter>(OtherActor);

	if (BaseCharacter)
	{
		// if (ASceneItemActor* SceneItemActor = Cast<ASceneItemActor>(OtherActor))
		// {
		// 	if(PackageComponent)
		// 	{
		// 		PackageComponent->AddNearItem(SceneItemActor);
		// 	}
		// }
		// BaseCharacter->SetOverlappingItem(nullptr);
	}
}



// Called every frame
void AItemWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

