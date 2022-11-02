// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_MagicProjectile.h"

#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AAR_MagicProjectile::AAR_MagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAR_MagicProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		auto* Attribute = Cast<UAR_AttributeComponent>(
			OtherActor->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
		if (Attribute)
		{
			Attribute->ApplyHealthChange(ProjectileDamage);
			Explode();

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AAR_MagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAR_MagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
