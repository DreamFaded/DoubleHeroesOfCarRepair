// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/DoubleHeroesInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "Interface/DoubleHeroesAbilitySystemInterface.h"
#include "Interface/InventoryInterface.h"
#include "DoubleHeroesPlayerController.generated.h"

class UDoubleHeroesSystemsWidget;
class UInventoryWidgetController;
class UInventoryComponent;
class UEquipmentManagerComponent;
class ADoubleHeroesBaseCharacter;
class USplineComponent;
class UDamageTextComponent;
class UInputMappingContext;
class UDoubleHeroesInputConfig;
struct FInputActionValue;
class UInputAction;
class UDataAsset_InputConfig;
/**
 * 
 */
UCLASS()
class DOUBLEHEROES_API ADoubleHeroesPlayerController : public APlayerController, public IAbilitySystemInterface, public IInventoryInterface, public IDoubleHeroesAbilitySystemInterface 
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void Server_SetMaxWalkSpeed(float NewSpeed);

	UPROPERTY()
	bool bTogglePackage = false;
	
	FVector2D MovementVector = FVector2D();

	FRotator MovementRotation = FRotator();

	FKey PressedKey = EKeys::Invalid;

	FKey LastPressedKey = EKeys::Invalid;

	double DoubleClickThreshold = 0.3;
	double CurrentTime = 0.0;
	double LastTime = 0.0;
	FVector Velocity = FVector::ZeroVector;
	bool bKeyA = false;
	bool bKeyS = false;
	bool bKeyW = false;
	bool bKeyD = false;

	ADoubleHeroesBaseCharacter* BaseCharacter;
	
	ADoubleHeroesPlayerController();
	
	virtual UInventoryComponent* GetInventoryComponent_Implementation() override;

	virtual void SetDynamicProjectile_Implementation(const FGameplayTag& ProjectileTag) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UInventoryWidgetController* GetInventoryWidgetController();

	void CreateInventoryWidget();

	void CreateWidgetController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;

	// virtual void SetupInputComponent() override;
	
	virtual void SetupInputComponent() override;
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_PressW(const FInputActionValue& InputActionValue);
	void Input_PressA(const FInputActionValue& InputActionValue);
	void Input_PressS(const FInputActionValue& InputActionValue);
	void Input_PressD(const FInputActionValue& InputActionValue);
	void Input_ReleaseW(const FInputActionValue& InputActionValue);
	void Input_ReleaseA(const FInputActionValue& InputActionValue);
	void Input_ReleaseS(const FInputActionValue& InputActionValue);
	void Input_ReleaseD(const FInputActionValue& InputActionValue);
	void Input_SaveClick(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_StartRun();
	void Input_StopRun();
	void Input_OpenPackage();
	void Input_ClosePackage();
	void Input_AbilityInputPressed(FGameplayTag InInputTag);

	void BindCallbacksToDependencies();

private:

	UPROPERTY()
	APawn* ControlledPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	TObjectPtr<UDHAbilitySystemComponent> DHAbilitySystemComp;

	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditDefaultsOnly, Category="Custom|Widgets")
	TSubclassOf<UInventoryWidgetController> InventoryWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UDoubleHeroesSystemsWidget> InventoryWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="Custom|Widgets")
	TSubclassOf<UDoubleHeroesSystemsWidget> InventoryWidgetClass;

	UDHAbilitySystemComponent* GetDHAbilitySystemComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AbilityInputPressed(FGameplayTag InputTag);
	
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputMappingContext> DoubleHeroesContext;
	//
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputAction> MoveAction;
	//
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputAction> LookAction;
	//
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputAction> RunAction;
	//
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputAction> DodgeAction;
	//
	// UPROPERTY(EditAnywhere, Category = "Input")
	// TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	/*void Move(const FInputActionValue& InputActionValue);
	void Run(const FInputActionValue& InputActionValue);
	void Walk(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);*/
	// void AutoRun();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentManagerComponent> EquipmentComponent;
	
	void CursorTrace();

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;
	bool bIsRunning = false;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	// void AbilityInputTagReleased(FGameplayTag InputTag);
	// void AbilityInputTagHeld(FGameplayTag InputTag);
	
	// UPROPERTY(EditDefaultsOnly, Category = "Input")
	// TObjectPtr<UDoubleHeroesInputConfig> InputConfig;

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

