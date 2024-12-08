// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Subsystem/ItemSubsystem.h"
#include "SkinComponent.generated.h"

class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOUBLEHEROES_API USkinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkinComponent();

	void OnPutOnItem(ESkinPartType PartType, int32 ItemID);
	void OnTakeOffItem(ESkinPartType PartType, int32 ItemID);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<ESkinPartType, UStaticMeshComponent*> SMCMap;

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	FName GetSocketName(ESkinPartType PartType);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
