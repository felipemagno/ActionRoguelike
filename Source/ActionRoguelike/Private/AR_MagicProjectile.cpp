// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_MagicProjectile.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AAR_MagicProjectile::AAR_MagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");	
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;	

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectsComp");
	EffectComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void AAR_MagicProjectile::OnOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult)
{
	if(Actor)
	{
		auto* Attribute = Cast<UAR_AttributeComponent>( Actor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
		if(Attribute)
		{
			Attribute->ApplyHealthChange(-20);

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AAR_MagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&AAR_MagicProjectile::OnOverlap);
}

// Called every frame
void AAR_MagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

