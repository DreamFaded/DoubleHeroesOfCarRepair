// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesEnemy.h"

#include "AbilitySystemComponent.h"
#include "DoubleHeroesGameplayTags.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAbilitySystemLibrary.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "DoubleHeroes/DoubleHeroes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/DoubleHeroesUserWidget.h"


ADoubleHeroesEnemy::ADoubleHeroesEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void ADoubleHeroesEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ADoubleHeroesEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 ADoubleHeroesEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void ADoubleHeroesEnemy::Die()
{
	SetLifeSpan(5.f);
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
	Super::Die();
}

void ADoubleHeroesEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UDoubleHeroesAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	if (UDoubleHeroesUserWidget* DoubleHeroesUserWidget = Cast<UDoubleHeroesUserWidget>(
		HealthBar->GetUserWidgetObject()))
	{
		DoubleHeroesUserWidget->SetWidgetController(this);
	}

	if (const UDoubleHeroesAttributeSet* DoubleHeroesAS = Cast<UDoubleHeroesAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAS->GetMaxHealthAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnHealthChanged.Broadcast(Data.NewValue);
			                        }
		                        );
		AbilitySystemComponent->RegisterGameplayTagEvent(FDoubleHeroesGameplayTags::Get().Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &ADoubleHeroesEnemy::HitReactTagChanged);

		OnHealthChanged.Broadcast(DoubleHeroesAS->GetHealth());
		OnMaxHealthChanged.Broadcast(DoubleHeroesAS->GetMaxHealth());
	}
}

void ADoubleHeroesEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void ADoubleHeroesEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDHAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void ADoubleHeroesEnemy::InitializeDefaultAttributes() const
{
	UDoubleHeroesAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
