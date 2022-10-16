// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_ExplosiveBarrel.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"


void AAR_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AAR_ExplosiveBarrel::OnHit);
}

void AAR_ExplosiveBarrel::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                UPrimitiveComponent* PrimitiveComponent1, UE::Math::TVector<double> Vector,
                                const FHitResult& HitResult)
{
	if (Actor->ActorHasTag("Projectile"))
		Explode();
}

void AAR_ExplosiveBarrel::Explode()
{
	RadialForceComp->FireImpulse();	
}


// Sets default values
AAR_ExplosiveBarrel::AAR_ExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->SetupAttachment(StaticMeshComp);
}
