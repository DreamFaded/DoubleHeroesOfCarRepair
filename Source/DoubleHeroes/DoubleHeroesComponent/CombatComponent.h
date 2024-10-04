// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOUBLEHEROES_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class UBlueHeroCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
	class ABlueHeroCharacter* Character;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	class AWeapon* EquippedWeapon;
		
};
