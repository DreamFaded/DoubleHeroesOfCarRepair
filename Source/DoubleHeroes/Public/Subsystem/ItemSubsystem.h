// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

//区分装饰型道具和武器型道具
UENUM()
enum class EItemType : uint8
{
	EPT_SkinPart,
	EPT_Weapon
};

UENUM()
enum class ESkinPartType : uint8
{
	ESPT_None,
	ESPT_Cap,
	ESPT_Glasses,
	ESPT_Helmet,
	ESPT_Hair,
	ESPT_Clothes,
	ESPT_Pants,
	ESPT_Shoes,
	ESPT_Backpack,
	ESPT_Masker,
	ESPT_Headset,
	ESPT_Beard
};

USTRUCT()
struct FItemBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	int32 ID;
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	EItemType Type;
	
};

USTRUCT()
struct FSkinPart : public FItemBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	ESkinPartType PartType;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;
};
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FItemBase* GetItemData(int32 ID) const;

protected:

	UPROPERTY()
	UDataTable* ItemData;

	TMap<int32, FItemBase*> ItemMap;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
