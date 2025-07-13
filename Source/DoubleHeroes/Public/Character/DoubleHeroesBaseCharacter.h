// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "Components/PackageComponent.h"
#include "GameFramework/Character.h"
#include "Interface/ISkinInterface.h"
#include "DoubleHeroesBaseCharacter.generated.h"

class UDataAsset_StartUpDataBase;
class UDoubleHeroesAttributeSet;
class UDHAbilitySystemComponent;
class USkinComponent;
class UGameplayEffect;

UCLASS()
class DOUBLEHEROES_API ADoubleHeroesBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IISkinInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADoubleHeroesBaseCharacter(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsRunning = false;
	UPROPERTY(Replicated)
	bool bEquipped = false;
	UPROPERTY(Replicated)
	bool bIsStabilizing = false;

	UPROPERTY(Replicated)
	float MovementSpeedMultiplier;

	FVector2D MovementVector;

	FRotator MovementRotation;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 1200.f;

	UPROPERTY(EditAnywhere)
	float stabilityTime = 0.3f;
	
	FKey PressedKey = EKeys::Invalid;

	FKey LastPressedKey = EKeys::Invalid;

	FTimerHandle StabilizeTimerHandle;
	

	// Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface Interface

	bool IsRunning();

	void SetOverlappingWeapon(AWeapon* Weapon);

	UPackageComponent* GetPackageComponent() const {return PackageComponent;}
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	// virtual void Landed(const FHitResult& Hit) override;;
	
	// void Input_StartRun();
	// void Input_StopRun();
	// UFUNCTION(Server, Reliable)
	// void Server_StartRunning();
	// UFUNCTION(Server, Reliable)
	// void Server_StopRunning();
	
	
	AWeapon* GetHoldWeapon() const;

	FORCEINLINE UDHAbilitySystemComponent* GetDHAbilitySystemComponent() const { return DHAbilitySystemComponent; }

	FORCEINLINE UDoubleHeroesAttributeSet* GetDoubleHeroesAttributeSet() const { return DoubleHeroesAttributeSet; }

	//APawn Interface
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPackageComponent* PackageComponent;
	
	UPROPERTY(VisibleAnywhere)
	USkinComponent* SkinComponent;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;
	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", Replicated)
	UDHAbilitySystemComponent* DHAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDoubleHeroesAttributeSet> DoubleHeroesAttributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet;

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
	FGameplayTag CharacterTag;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DynamicProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;


	virtual void InitializeDefaultAttributes() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

	virtual USkeletalMeshComponent* GetSkeletalMeshComponent() override;

private:

	// UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	// TObjectPtr<UDHAbilitySystemComponent> DHAbilitySystemComponent;

	void InitAbilityActorInfo();

	void InitClassDefaults();

	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

};
