// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_BaseProjectile.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AAR_BaseProjectile::AAR_BaseProjectile()
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
	{
		LoopSoundComponent->SetSound(LoopSFX);
	}

	ShakeInnerRadius = 250;
	ShakeOuterRadius = 2500;
}

void AAR_BaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAR_BaseProjectile::OnOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &AAR_BaseProjectile::OnHit);
}

// Called when the game starts or when spawned
void AAR_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	if (LoopSFX)
	{
		LoopSoundComponent->SetSound(LoopSFX);
	}

	if (MuzzleEffect)
	{
		auto* InstigatorCharacter = Cast<ACharacter>(GetInstigator());
		if (InstigatorCharacter)
		{
			auto Mesh = InstigatorCharacter->GetMesh();
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, Mesh,TEXT("Muzzle_01"));
		}
	}
}

// Called every frame
void AAR_BaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAR_BaseProjectile::Explode()
{
	if (HitSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, GetActorLocation(), GetActorRotation());
	}
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect,
		                                         FTransform(GetActorRotation(), GetActorLocation()));
	}
	if (Shake)
	{
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), Shake, GetActorLocation(), ShakeInnerRadius,
		                                       ShakeOuterRadius);
	}
}

void AAR_BaseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
	Destroy();
}

void AAR_BaseProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
}
