// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Initial UMETA(DisplayName = "Initial State"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Dropped UMETA(DisplayName = "Dropped"),
	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS(Abstract, Blueprintable)
class DOUBLEHEROES_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();

	UPROPERTY(ReplicatedUsing = OnRep_ItemState, VisibleAnywhere, Category = "Item Properties", BlueprintReadOnly)
	EItemState ItemState;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ItemID;

	// 耐久度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float Durability;

	// 最大耐久度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MaxDurability;

	// 道具 Tag（用于标识类型）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FGameplayTag ItemTag;


	// 网络同步
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called when durability changes
	// UPROPERTY(BlueprintAssignable, Category = "Item")
	// FOnDurabilityChanged OnDurabilityChanged;

	// 使用道具
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void UseItem();

	// 使用道具
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void Destroyed() override;

	int64 GenerateID();

	void SetItemState(EItemState State);

	UFUNCTION()
	void OnRep_ItemState();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Properties")
	class UBoxComponent* AreaBox;

	UFUNCTION()
	virtual void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:
	virtual void Tick(float DeltaTime) override;
};
