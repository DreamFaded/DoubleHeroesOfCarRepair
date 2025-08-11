// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoubleHeroesBaseCharacter.h"
#include "DoubleHeroesCharacterBase.h"
#include "BlueHeroCharacter.generated.h"

class UInteractionComponent;
class UHeroCombatComponent;
class UDataAsset_InputConfig;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UCLASS()
class DOUBLEHEROES_API ABlueHeroCharacter : public ADoubleHeroesBaseCharacter
{
	GENERATED_BODY()
	
	/** Jump Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// class UInputAction* JumpAction;
	
	/** Look Input Action */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// class UInputAction* LookAction;

	

public:
	// Sets default values for this character's properties
	ABlueHeroCharacter(const FObjectInitializer& ObjectInitializer);
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; } 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
	bool IsWeaponEquipped();

	// class UCombatComponent* Combat;
	// UPROPERTY(EditAnywhere, Category = input)
	// class UInputMappingContext* SlashContext;


protected:
	
	virtual void PossessedBy(AController* NewController) override;
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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


	// void Input_Move(const FInputActionValue& InputActionValue);
	// void Input_Look(const FInputActionValue& InputActionValue);
	// void Input_TogglePackage(const FInputActionValue& InputActionValue);
	// void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	void Punch();
	void Dodge();
	void Interact();
	void CrouchPressed();

	void LoadProgress();



private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;
	
	// UInteractionComponent* InteractionComponent;

	

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	// UDataAsset_InputConfig* InputConfigDataAsset;


	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	

};
