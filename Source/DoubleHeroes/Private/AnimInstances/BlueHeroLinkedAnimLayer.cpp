// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BlueHeroLinkedAnimLayer.h"

#include "AnimInstances/BlueHeroAnimInstance.h"

UBlueHeroAnimInstance* UBlueHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
		return Cast<UBlueHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
