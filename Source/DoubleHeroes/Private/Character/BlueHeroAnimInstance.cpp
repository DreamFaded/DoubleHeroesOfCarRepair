// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlueHeroAnimInstance.h"

#include "Character/BlueHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlueHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlueHeroCharacter = Cast<ABlueHeroCharacter>(TryGetPawnOwner());
}

void UBlueHeroAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlueHeroCharacter == nullptr)
	{
		BlueHeroCharacter = Cast<ABlueHeroCharacter>(TryGetPawnOwner());
	}
	if(BlueHeroCharacter == nullptr) return;

	FVector Velocity = BlueHeroCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bIsInAir = BlueHeroCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BlueHeroCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = BlueHeroCharacter->IsWeaponEquipped();
	bIsCrouched = BlueHeroCharacter->bIsCrouched;
	//Offset Yaw for Strafing
	/*FRotator AimRotation = BlueHeroCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlueHeroCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRotation.Yaw;
	if (!BlueHeroCharacter->HasAuthority() && !BlueHeroCharacter->IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("AimRotation Yaw %f: "),AimRotation.Yaw)
		UE_LOG(LogTemp, Warning, TEXT("MovementRotation Yaw %f: "),MovementRotation.Yaw);
	}
	// 保存上一帧的角色旋转
	CharacterRotationLastFrame = CharacterRotation;
	// 获取当前帧的角色旋转
	CharacterRotation = BlueHeroCharacter->GetActorRotation();
	// 计算当前帧与上一帧之间的旋转差值
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	// 计算目标偏转角度（每秒的偏转量）
	const float Target = Delta.Yaw / DeltaTime;
	// 对 Lean 进行插值操作，使其变化更加平滑
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	// 将插值结果限制在 -90 到 90 度之间
	Lean = FMath::Clamp(Interp, -90.f, 90.f);*/
}
