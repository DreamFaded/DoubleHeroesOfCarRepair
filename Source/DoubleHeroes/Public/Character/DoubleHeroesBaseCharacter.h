// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Components/PackageComponent.h"
#include "GameFramework/Character.h"
#include "DoubleHeroesBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	FVector2D MovementVector;

	FRotator MovementRotation;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 1200.f;
	
	FKey PressedKey = EKeys::Invalid;

	FKey LastPressedKey = EKeys::Invalid;
	
	// Sets default values for this character's properties
	ADoubleHeroesBaseCharacter();

	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	bool IsRunning();

	void SetOverlappingWeapon(AWeapon* Weapon);

	UPackageComponent* GetPackageComponent() const {return PackageComponent;}
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_StartRun();
	void Input_StopRun();

protected:
	UPROPERTY(VisibleAnywhere)
	UPackageComponent* PackageComponent;

	bool bRunning;
	bool bEquipped;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	//Begin APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDHAbilitySystemComponent* DHAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE UDHAbilitySystemComponent* GetDHAbilitySystemComponent() const { return DHAbilitySystemComponent; }

	FORCEINLINE UDoubleHeroesAttributeSet* GetDoubleHeroesAttributeSet() const { return DoubleHeroesAttributeSet; }

private:
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

};
