// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlueHeroCharacter.generated.h"

UCLASS()
class DOUBLEHEROES_API ABlueHeroCharacter : public ACharacter
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
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();

	class UCombatComponent* Combat;
	UPROPERTY(EditAnywhere, Category = input)
	class UInputMappingContext* SlashContext;

protected:
	// APawn interface
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	UPROPERTY(EditAnywhere, Category = input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = input)
	UInputAction* PunchAction;
	UPROPERTY(EditAnywhere, Category = input)
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, Category = input)
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, Category = input)
	UInputAction* CrouchAction;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Punch();
	void Dodge();
	void Interact();
	void CrouchPressed();

	void MoveForward(float Value);


private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	

};
