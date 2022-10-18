// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_ExplosiveBarrel.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"


void AAR_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AAR_ExplosiveBarrel::OnHit(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                UPrimitiveComponent* PrimitiveComponent1, UE::Math::TVector<double> Vector,
                                const FHitResult& HitResult)
{
	if (Actor->ActorHasTag("Projectile"))
	{
		Explode();

		// %s string
		// %f float
		UE_LOG(LogTemp,Warning,TEXT("Explode by OtherActor: %s, at game time: %f"),*GetNameSafe(Actor),GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *HitResult.ImpactPoint.ToString());
		DrawDebugString(GetWorld(),HitResult.ImpactPoint,CombinedString, nullptr,FColor::Green,3,true,2);
	}
}

void AAR_ExplosiveBarrel::Explode()
{
	RadialForceComp->FireImpulse();
	
	UE_LOG(LogTemp,Log,TEXT("Barrel Explode"));	
}

void AAR_ExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AAR_ExplosiveBarrel::OnHit);
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
