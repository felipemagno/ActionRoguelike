// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_TeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

bool TeleportFromHit = false;

// Sets default values
AAR_TeleportProjectile::AAR_TeleportProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAR_TeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeParticle, this, &AAR_TeleportProjectile::ExplodeParticle,
	                                ProjectileLifetime);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AAR_TeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AAR_TeleportProjectile::OnHit);
}

void AAR_TeleportProjectile::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                   UPrimitiveComponent* PrimitiveComponent1, UE::Math::TVector<double> Vector,
                                   const FHitResult& HitResult)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ExplodeParticle);
	TeleportFromHit = true;
	ExplodeParticle();
}

void AAR_TeleportProjectile::TeleportInstigator()
{
	auto TeleportTarget = GetInstigator();
	if (TeleportTarget)
	{
		auto TeleportLocation = TeleportFromHit
			                        ? GetActorLocation() - GetActorForwardVector() * TeleportHitOffset
			                        : GetActorLocation();
		TeleportTarget->TeleportTo(TeleportLocation, TeleportTarget->GetControlRotation());
	}
	Destroy();
}

void AAR_TeleportProjectile::ExplodeParticle()
{
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), GetActorRotation());
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportParticle, this, &AAR_TeleportProjectile::TeleportInstigator,
	                                TeleportDelay);
}

// Called every frame
void AAR_TeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
