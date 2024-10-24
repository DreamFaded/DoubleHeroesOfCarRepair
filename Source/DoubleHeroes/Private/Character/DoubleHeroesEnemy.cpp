// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "DoubleHeroes/DoubleHeroes.h"
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

int32 ADoubleHeroesEnemy::GetPlayerLevel()
{
	return Level;
}

void ADoubleHeroesEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if(UDoubleHeroesUserWidget* DoubleHeroesUserWidget = Cast<UDoubleHeroesUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		DoubleHeroesUserWidget->SetWidgetController(this);
	}
	
	if(const UDoubleHeroesAttributeSet* DoubleHeroesAS = Cast<UDoubleHeroesAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
			);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DoubleHeroesAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
			);
		OnHealthChanged.Broadcast(DoubleHeroesAS->GetHealth());
		OnMaxHealthChanged.Broadcast(DoubleHeroesAS->GetMaxHealth());
	}
}

void ADoubleHeroesEnemy::InitAbilityActorInfo()
{

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDHAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttribute();
}
