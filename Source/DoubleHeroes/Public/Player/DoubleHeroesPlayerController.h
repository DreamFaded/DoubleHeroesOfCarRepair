// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/DoubleHeroesInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "DoubleHeroesPlayerController.generated.h"

class USplineComponent;
class UDamageTextComponent;
class UInputMappingContext;
class UDoubleHeroesInputConfig;
struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADoubleHeroesPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DoubleHeroesContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	/*void Move(const FInputActionValue& InputActionValue);
	void Run(const FInputActionValue& InputActionValue);
	void Walk(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);*/
	void AutoRun();

	bool bShiftKeyDown = false;
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };

	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	bool bIsRunning = false;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDoubleHeroesInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UDHAbilitySystemComponent> DHAbilitySystemComponent;

	UDHAbilitySystemComponent* GetASC();

	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};

