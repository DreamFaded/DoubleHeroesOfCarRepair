// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneItemActor.generated.h"

UCLASS()
class DOUBLEHEROES_API ASceneItemActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneItemActor();

	int32 GetItemID() const { return ItemID; }

	void SetID(int32 OutID, bool bInit);
	void SetItemID(int32 ID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Init();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 ID;

	UPROPERTY(EditAnywhere)
	int32 ItemID;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;
};
