// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_ExplosiveBarrel.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "ActorComponent/AR_InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"

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

	ExplosionDamage = -35;
}

void AAR_ExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &AAR_ExplosiveBarrel::OnHit);
}

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
		UE_LOG(LogTemp, Warning, TEXT("Explode by OtherActor: %s, at game time: %f"), *GetNameSafe(Actor),
		       GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *HitResult.ImpactPoint.ToString());
		DrawDebugString(GetWorld(), HitResult.ImpactPoint, CombinedString, nullptr, FColor::Green, 3, true, 2);
	}
}

void AAR_ExplosiveBarrel::Explode()
{
	RadialForceComp->FireImpulse();

	{
		TArray<FHitResult> HitResults;
		TArray<FOverlapResult> OverlapResults;
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(RadialForceComp->Radius);

		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		bool bBlockingHit = GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(), FQuat::Identity,
		                                                         QueryParams, CollisionShape);
		DrawDebugSphere(GetWorld(),GetActorLocation(),RadialForceComp->Radius,32,FColor::Black,false,4,0,2);

		TArray<AActor*> ActorsHit;
		
		for (FOverlapResult Overlap : OverlapResults)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && !ActorsHit.Contains(HitActor))
			{
				ActorsHit.Add(HitActor);
				auto* AttributeComp = Cast<UAR_AttributeComponent>(HitActor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
				if(AttributeComp)
				{
					AttributeComp->ApplyHealthChange(ExplosionDamage);
				}
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Barrel Explode"));
}
