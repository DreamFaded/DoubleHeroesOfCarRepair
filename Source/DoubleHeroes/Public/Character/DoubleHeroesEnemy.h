// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesCharacterBase.h"
#include "Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "DoubleHeroesEnemy.generated.h"

class ADoubleHeroesAIController;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesEnemy : public ADoubleHeroesCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	ADoubleHeroesEnemy();

	virtual void PossessedBy(AController* NewController) override;

	//Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	//Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHightlighted = false;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Caombat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ADoubleHeroesAIController> DoubleHeroesAIController;
private:

	
	
};
