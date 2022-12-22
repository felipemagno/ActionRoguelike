// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AR_MagicProjectile.h"

#include "ActorComponent/AR_ActionComponent.h"
#include "ActorComponent/AR_AttributeComponent.h"
#include "Components/AudioComponent.h"
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
}

void AAR_MagicProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAR_ActionComponent* ActionComponent = Cast<UAR_ActionComponent>(
		OtherActor->GetComponentByClass(UAR_ActionComponent::StaticClass()));
	// if (ActionComponent)
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange,
	// 	                                 "Projectile Overlap: Projectile Parry Tag (" + ParryTag.ToString() +
	// 	                                 "); OtherActorTag: (" + ActionComponent->ActiveGameplayTags.ToStringSimple() +
	// 	                                 ");");

	if (OtherActor && OtherActor != GetInstigator())
	{
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			// FString DebugText;
			// TArray<AActor*> IgnoreList = SphereComp->GetMoveIgnoreActors();
			// for (AActor* IgnoredActor : IgnoreList)
			// {
			// 	DebugText += IgnoredActor->GetName() + "; ";
			// }
			//
			// UE_LOG(LogTemp, Log, TEXT("%s Projectile Debug#1: HitComp- %s, HitActor- %s"), *this->GetName(),
			//        *OtherComp->GetName(), *OtherActor->GetName())
			//
			// UE_LOG(LogTemp, Log, TEXT("%s Projectile Debug#2: IgnoredActors - %s"), *this->GetName(), *DebugText)

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
