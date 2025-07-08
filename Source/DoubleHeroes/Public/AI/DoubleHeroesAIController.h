// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DoubleHeroesAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoubleHeroesAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
