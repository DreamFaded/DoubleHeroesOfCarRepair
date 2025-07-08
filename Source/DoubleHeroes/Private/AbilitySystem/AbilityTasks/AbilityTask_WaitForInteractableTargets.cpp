// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitForInteractableTargets.h"

#include "AbilitySystemComponent.h"
#include "Interaction/InteractableTarget.h"

void UAbilityTask_WaitForInteractableTargets::LineTrace(FHitResult& OutHitResult, const UWorld* World,
	const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	//清空输出结果，确保初始状态干净
	OutHitResult = FHitResult();
	//定义一个数组来保存所有命中结果
	TArray<FHitResult> HitResults;
	//使用给定的碰撞配置文件执行线性追踪，并将所有命中结果保存到 HitResults 数组中
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	//如果有命中结果，则将第一个命中的结果赋值给 OutHitResult
	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
}

//基于玩家控制器的视角方向进行瞄准计算
void UAbilityTask_WaitForInteractableTargets::AimWithPlayerController(const AActor* InSourceActor,
	FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd,
	bool bIgnorePitch) const
{
	//只有服务器和启动客户端才执行，防止在无效的 Ability 上操作
	if (!Ability) // Server and launching client only
	{
		return;
	}

	//@TODO: Bots?
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	//获取玩家摄像机的位置 (ViewStart) 和旋转角度 (ViewRot)
	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);

	const FVector ViewDir = ViewRot.Vector();
	//将旋转转换为方向向量，并延伸到最大范围，形成一条射线终点
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	//限制射线不超过技能作用范围球体（Sphere），防止超出能力范围
	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);

	//从视角出发执行一次线性追踪，查看是否有阻挡物或可交互对象
	FHitResult HitResult;
	LineTrace(HitResult, InSourceActor->GetWorld(), ViewStart, ViewEnd, TraceProfile.Name, Params);

	//如果命中了物体，且命中点在技能范围内，则使用命中点作为终点
	const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange));

	//根据命中点或原始终点计算瞄准方向。
	const FVector AdjustedEnd = (bUseTraceResult) ? HitResult.Location : ViewEnd;
	//如果方向为零向量（无法标准化），则使用原始方向
	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	//若设置了不改变俯仰角，则保留原始视角的 Pitch，仅修改 Yaw（水平方向）
	if (!bTraceAffectsAimPitch && bUseTraceResult)
	{
		FVector OriginalAimDir = (ViewEnd - TraceStart).GetSafeNormal();

		if (!OriginalAimDir.IsZero())
		{
			// Convert to angles and use original pitch
			const FRotator OriginalAimRot = OriginalAimDir.Rotation();

			FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
			AdjustedAimRot.Pitch = OriginalAimRot.Pitch;

			AdjustedAimDir = AdjustedAimRot.Vector();
		}
	}

	//将最终计算出的方向延伸到最大范围，得到最终的瞄准终点
	OutTraceEnd = TraceStart + (AdjustedAimDir * MaxRange);
}

bool UAbilityTask_WaitForInteractableTargets::ClipCameraRayToAbilityRange(FVector CameraLocation,
	FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DotToCenter >= 0)		//If this fails, we're pointed away from the center, but we might be inside the sphere and able to find a good exit point.
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;						//Subtracting instead of adding will get the other intersection point
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);		//Cam aim point clipped to range sphere
			return true;
		}
	}
	return false;
}

//更新当前可交互对象的可用交互选项
void UAbilityTask_WaitForInteractableTargets::UpdateInteractableOptions(const FInteractionQuery& InteractQuery,
	const TArray<TScriptInterface<IInteractableTarget>>& InteractableTargets)
{
	//TArray<FInteractionOption> NewOptions;
	TArray<FInteractionOption> NewOptions;

	for (const TScriptInterface<IInteractableTarget>& InteractiveTarget : InteractableTargets)
	{
		TArray<FInteractionOption> TempOptions;
		FInteractionOptionBuilder InteractionBuilder(InteractiveTarget, TempOptions);
		//对每个可交互对象调用 GatherInteractionOptions() 方法，收集它支持的所有交互选项
		InteractiveTarget->GatherInteractionOptions(InteractQuery, InteractionBuilder);

		for (FInteractionOption& Option : TempOptions)
		{
			FGameplayAbilitySpec* InteractionAbilitySpec = nullptr;

			// if there is a handle and a target ability system, we're triggering the ability on the target.
			//情况一：目标上已有 Ability Handle
			if (Option.TargetAbilitySystem && Option.TargetInteractionAbilityHandle.IsValid())
			{
				// Find the spec
				InteractionAbilitySpec = Option.TargetAbilitySystem->FindAbilitySpecFromHandle(Option.TargetInteractionAbilityHandle);
			}
			// If there's an interaction ability then we're activating it on ourselves.
			//情况二：本地要授予一个 Ability
			else if (Option.InteractionAbilityToGrant)
			{
				// Find the spec
				InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Option.InteractionAbilityToGrant);

				if (InteractionAbilitySpec)
				{
					// update the option
					Option.TargetAbilitySystem = AbilitySystemComponent.Get();
					Option.TargetInteractionAbilityHandle = InteractionAbilitySpec->Handle;
				}
			}

			if (InteractionAbilitySpec)
			{
				// Filter any options that we can't activate right now for whatever reason.
				//只保留那些当前可以激活的交互选项
				if (InteractionAbilitySpec->Ability->CanActivateAbility(InteractionAbilitySpec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
				{
					NewOptions.Add(Option);
				}
			}
		}
	}

	//比较新旧选项，判断是否发生变化
	bool bOptionsChanged = false;
	if (NewOptions.Num() == CurrentOptions.Num())
	{
		NewOptions.Sort();

		for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
		{
			const FInteractionOption& NewOption = NewOptions[OptionIndex];
			const FInteractionOption& CurrentOption = CurrentOptions[OptionIndex];

			//排序后逐个比较内容
			if (NewOption != CurrentOption)
			{
				bOptionsChanged = true;
				break;
			}
		}
	}
	//如果数量不同，直接认为有变化
	else
	{
		bOptionsChanged = true;
	}

	if (bOptionsChanged)
	{
		//更新当前交互选项列表并广播
		CurrentOptions = NewOptions;
		InteractableObjectsChanged.Broadcast(CurrentOptions);
	}
}
