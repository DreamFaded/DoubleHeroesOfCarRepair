// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DoubleHeroesEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/DHAbilitySystemComponent.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"
#include "DoubleHeroes/DoubleHeroes.h"


ADoubleHeroesEnemy::ADoubleHeroesEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UDHAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UDoubleHeroesAttributeSet>(TEXT("AttributeSet"));
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

void ADoubleHeroesEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void ADoubleHeroesEnemy::InitAbilityActorInfo()
{

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDHAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
