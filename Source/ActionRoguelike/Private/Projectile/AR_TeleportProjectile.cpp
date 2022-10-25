// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_TeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	GetWorldTimerManager().SetTimer(TimerHandle_ExplodeParticle, this, &AAR_TeleportProjectile::Explode,
	                                ProjectileLifetime);
}

void AAR_TeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AAR_TeleportProjectile::OnHit);
}

// Called every frame
void AAR_TeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAR_TeleportProjectile::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                   UPrimitiveComponent* PrimitiveComponent1, UE::Math::TVector<double> Vector,
                                   const FHitResult& HitResult)
{	
	GetWorldTimerManager().ClearTimer(TimerHandle_ExplodeParticle);
	TeleportFromHit = true;
	Explode();
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


void AAR_TeleportProjectile::Explode()
{
	Super::Explode();
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MovementComp->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportParticle, this, &AAR_TeleportProjectile::TeleportInstigator,
									TeleportDelay);
}