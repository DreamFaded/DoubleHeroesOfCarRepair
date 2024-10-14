// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DoubleHeroesEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DoubleHeroesAttributeSet.h"


// Sets default values
ADoubleHeroesEffectActor::ADoubleHeroesEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ADoubleHeroesEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                         const FHitResult& SweepResult)
{
	//TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a heck!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UDoubleHeroesAttributeSet* DoubleHeroesAttributeSet = Cast<UDoubleHeroesAttributeSet>(
			ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UDoubleHeroesAttributeSet::StaticClass()));
		UDoubleHeroesAttributeSet* MutableDoubleHeroesAttributeSet = const_cast<UDoubleHeroesAttributeSet*>(DoubleHeroesAttributeSet);
		MutableDoubleHeroesAttributeSet->SetHealth(DoubleHeroesAttributeSet->GetHealth() + 25.f);
		MutableDoubleHeroesAttributeSet->SetEndurance(DoubleHeroesAttributeSet->GetEndurance() - 25.f);
		Destroy();
	}
}

void ADoubleHeroesEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
}

// Called when the game starts or when spawned
void ADoubleHeroesEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADoubleHeroesEffectActor::OnOverlap);
}
