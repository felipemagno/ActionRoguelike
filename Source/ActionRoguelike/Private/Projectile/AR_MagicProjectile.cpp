// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_MagicProjectile.h"

#include "Actions/AR_BaseActionEffect.h"
#include "ActorComponent/AR_ActionComponent.h"
#include "Components/SphereComponent.h"
#include "Core/AR_GameplayFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AAR_MagicProjectile::AAR_MagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);	
}

void AAR_MagicProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAR_ActionComponent* ActionComponent = Cast<UAR_ActionComponent>(
		OtherActor->GetComponentByClass(UAR_ActionComponent::StaticClass()));

	if (OtherActor && OtherActor != GetInstigator())
	{
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{

			MovementComp->Velocity = -MovementComp->Velocity;

			// Exchange OldInstigator to NewInstigator and update IgnoreActor set
			SphereComp->IgnoreActorWhenMoving(GetInstigator(), false);
			SphereComp->IgnoreActorWhenMoving(OtherActor, true);
			SetInstigator(Cast<APawn>(OtherActor));
		}
		else if (UAR_GameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, ProjectileDamage,
		                                                             SweepResult))
		{
			Explode();

			if(ProjectileEffect && ActionComponent && HasAuthority())
			{
				ActionComponent->AddAction(GetInstigator(),ProjectileEffect);
			}
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
