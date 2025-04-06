// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "Data/ProjectileInfo.h"
#include "Interface/DoubleHeroesAbilitySystemInterface.h"
#include "Libraries/DHAbilitySystemLibrary.h"
#include "Projectiles/ProjectileBase.h"

UProjectileAbility::UProjectileAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UProjectileAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AvatarActorFromInfo = GetAvatarActorFromActorInfo();

	if(!ProjectileToSpawnTag.IsValid() || !IsValid(AvatarActorFromInfo)) return;

	if (UProjectileInfo* ProjectileInfo = UDHAbilitySystemLibrary::GetProjectileInfo(AvatarActorFromInfo))
	{
		CurrentProjectileParams = *ProjectileInfo->ProjectileInfoMap.Find(ProjectileToSpawnTag);
	}
}

void UProjectileAbility::SpawnProjectile()
{
	if(!IsValid(CurrentProjectileParams.ProjectileClass)) return;

	if (USceneComponent* SpawnPointComp = IDoubleHeroesAbilitySystemInterface::Execute_GetDynamicSpawnPoint(
		AvatarActorFromInfo))
	{
		const FVector SpawnPoint = SpawnPointComp->GetComponentLocation();
		const FVector TargetLocation = AvatarActorFromInfo->GetActorLocation() * 10000;
		const FRotator TargetRotation = (TargetLocation - SpawnPoint).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnPoint);
		SpawnTransform.SetRotation(TargetRotation.Quaternion());

		if (AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectileBase>(
			CurrentProjectileParams.ProjectileClass, SpawnTransform, AvatarActorFromInfo))
		{
			SpawnedProjectile->SetProjectileParams(CurrentProjectileParams);

			FDamageEffectInfo DamageEffectInfo;
			CaptureDamageEffectInfo(nullptr,DamageEffectInfo);

			SpawnedProjectile->DamageEffectInfo = DamageEffectInfo;

			SpawnedProjectile->FinishSpawning(SpawnTransform);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Spawning Projectile %s"), *CurrentProjectileParams.ProjectileClass->GetName()));
}
