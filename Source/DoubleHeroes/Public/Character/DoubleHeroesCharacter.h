// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesBaseCharacter.h"
#include "AnimInstances/DoubleHeroesAnimInstance.h"
#include "Interface/DoubleHeroesAbilitySystemInterface.h"
#include "DoubleHeroesCharacter.generated.h"

class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;
struct FInputActionValue;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UDataAsset_InputConfig;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesCharacter : public ADoubleHeroesBaseCharacter, public IDoubleHeroesAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Combat Interface
	virtual int32 GetPlayerLevel_Implementation();

	//DoubleHeroesAbilitySystemInterface
	virtual USceneComponent* GetDynamicSpawnPoint_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float CurrentHealth, float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnduranceChanged(float CurrentEndurance, float MaxEndurance);

protected:
	// virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;
	
	
	// UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<USceneComponent> DynamicProjectileSpawnPoint;

	// void Input_Move(const FInputActionValue& InputActionValue);
	// void Input_Look(const FInputActionValue& InputActionValue);
	
	void InitAbilityActorInfo();
	void InitClassDefaults();

	void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();

};
