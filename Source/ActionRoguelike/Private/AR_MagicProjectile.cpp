// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_MagicProjectile.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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

	LoopSoundComponent = CreateDefaultSubobject<UAudioComponent>("LoopSoundComponent");
	LoopSoundComponent->SetupAttachment(SphereComp);
	if (LoopSFX)
		LoopSoundComponent->SetSound(LoopSFX);
}

void AAR_MagicProjectile::OnOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                    UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg,
                                    const FHitResult& HitResult)
{
	if (Actor)
	{
		auto* Attribute = Cast<UAR_AttributeComponent>(
			Actor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
		if (Attribute)
		{
			Attribute->ApplyHealthChange(-20);
			if (HitSFX)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, GetActorLocation(), GetActorRotation());

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AAR_MagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAR_MagicProjectile::OnOverlap);
	if (LoopSFX)
		LoopSoundComponent->SetSound(LoopSFX);

	if (MuzzleEffect)
	{
		auto* InstigatorCharacter = Cast<ACharacter>(GetInstigator());
		if (InstigatorCharacter)
		{
			auto Mesh = InstigatorCharacter->GetMesh();
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect,Mesh,TEXT("Muzzle_01"));	
		}		
	}
}

// Called every frame
void AAR_MagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
