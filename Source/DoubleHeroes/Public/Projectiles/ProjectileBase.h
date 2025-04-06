// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UBoxComponent;
class UDamageTextComponent;
class USphereComponent;
struct FProjectileParams;
class UProjectileMovementComponent;

UCLASS()
class DOUBLEHEROES_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	void SetProjectileParams(const FProjectileParams& Params);

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectInfo DamageEffectInfo;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY()
	TObjectPtr<UDamageTextComponent> DamageTextComponent;

};
