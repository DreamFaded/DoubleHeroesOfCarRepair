// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesBaseCharacter.h"
#include "DoubleHeroesCharacterBase.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlueHeroCharacter.generated.h"

class UHeroCombatComponent;
class UDataAsset_InputConfig;

UCLASS()
class DOUBLEHEROES_API ABlueHeroCharacter : public ADoubleHeroesBaseCharacter
{
	GENERATED_BODY()
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	

public:
	// Sets default values for this character's properties
	ABlueHeroCharacter();
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; } 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
	// void SetOverlappingWeapon(AWeapon* Weapon);
	// bool IsWeaponEquipped();

	// class UCombatComponent* Combat;
	UPROPERTY(EditAnywhere, Category = input)
	class UInputMappingContext* SlashContext;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// APawn interface
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	// UPROPERTY(EditAnywhere, Category = input)
	// UInputAction* MovementAction;
	// UPROPERTY(EditAnywhere, Category = input)
	// UInputAction* PunchAction;
	// UPROPERTY(EditAnywhere, Category = input)
	// UInputAction* DodgeAction;
	// UPROPERTY(EditAnywhere, Category = input)
	// UInputAction* InteractAction;
	// UPROPERTY(EditAnywhere, Category = input)
	// UInputAction* CrouchAction;


	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	void Punch();
	void Dodge();
	void Interact();
	void CrouchPressed();



private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	

};
