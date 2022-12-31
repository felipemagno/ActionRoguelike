// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_BlackHoleProjectile.h"


// Sets default values
AAR_BlackHoleProjectile::AAR_BlackHoleProjectile()
{	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->Radius = 1000.0f;
	RadialForceComponent->ForceStrength = -600000.0f;
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AAR_BlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAR_BlackHoleProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
		Explode();
	}
}

// Called every frame
void AAR_BlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

