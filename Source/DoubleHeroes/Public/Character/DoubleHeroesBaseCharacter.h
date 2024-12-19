// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Components/PackageComponent.h"
#include "GameFramework/Character.h"
#include "Interface/ISkinInterface.h"
#include "DoubleHeroesBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;
class USkinComponent;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IISkinInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesBaseCharacter();
	
	UPROPERTY(ReplicatedUsing=OnRep_Run, BlueprintReadOnly)
	bool bIsRunning = false;
	UPROPERTY(Replicated)
	bool bEquipped = false;

	UPROPERTY(Replicated)
	float MovementSpeedMultiplier;

	FVector2D MovementVector;

	FRotator MovementRotation;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 1200.f;
	
	FKey PressedKey = EKeys::Invalid;

	FKey LastPressedKey = EKeys::Invalid;
	

	// Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface Interface

	bool IsRunning();

	void SetOverlappingWeapon(AWeapon* Weapon);

	UPackageComponent* GetPackageComponent() const {return PackageComponent;}
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_StartRun();
	void Input_StopRun();
	
	UFUNCTION()
	virtual void OnRep_Run();
	
	AWeapon* GetHoldWeapon() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPackageComponent* PackageComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkinComponent* SkinComponent;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;
	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", Replicated)
	UDHAbilitySystemComponent* DHAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UFUNCTION(Server, Reliable)
	void Server_SetRunning(bool bNewRunning);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

	//Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	//End APawn Interface
	
	virtual void BeginPlay() override;

	virtual USkeletalMeshComponent* GetSkeletalMeshComponent() override;


public:
	FORCEINLINE UDHAbilitySystemComponent* GetDHAbilitySystemComponent() const { return DHAbilitySystemComponent; }

	FORCEINLINE UDoubleHeroesAttributeSet* GetDoubleHeroesAttributeSet() const { return DoubleHeroesAttributeSet; }

private:
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

};
