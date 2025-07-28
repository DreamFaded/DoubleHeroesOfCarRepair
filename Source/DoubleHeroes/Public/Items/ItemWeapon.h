// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "DoubleHeroesTypes/DoubleHeroesStructTypes.h"
#include "ItemWeapon.generated.h"

UCLASS()
class DOUBLEHEROES_API AItemWeapon : public AItemBase
{
	GENERATED_BODY()

public:
	AItemWeapon();

	// 攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackPower = 20;
	
	virtual void UseItem() override;

protected:
	virtual void BeginPlay() override;

	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	virtual void Tick(float DeltaTime) override;
};
