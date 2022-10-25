// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_InteractionComponent.h"

#include "Interfaces/AR_IGameplayInterface.h"


// Sets default values for this component's properties
UAR_InteractionComponent::UAR_InteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAR_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAR_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAR_InteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EndLocation;

	FVector EyeLocation;
	FRotator EyeRotation;
	AActor* Owner = GetOwner();
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	EndLocation = EyeLocation + EyeRotation.Vector() * 1000;

	// Simple single object Hit trace
	{
		// FHitResult Hit;
		// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, EndLocation, ObjectQueryParams);
		//
		// AActor* HitActor = Hit.GetActor();
		// if (HitActor && HitActor->Implements<UAR_IGameplayInterface>())
		// {
		// 	APawn* OwnerPawn = Cast<APawn>(Owner);
		//
		// 	IAR_IGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
		// }
		// if (bIsDebugging)
		// {
		// 	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
		// 	DrawDebugLine(GetWorld(), EyeLocation, EndLocation, LineColor, false, 3, 0, 2);
		// }
	}

	// Complex multi object with shape trace
	{
		TArray<FHitResult> HitResults;
		float traceRadius = 30.0f;
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(traceRadius);

		bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, EndLocation, FQuat::Identity,
		                                                       ObjectQueryParams,
		                                                       CollisionShape);

		for (FHitResult hit : HitResults)
		{
			AActor* HitActor = hit.GetActor();
			if (HitActor)
			{
				FColor DebugSphereColor;
				if (HitActor->Implements<UAR_IGameplayInterface>())
				{
					APawn* OwnerPawn = Cast<APawn>(Owner);
					IAR_IGameplayInterface::Execute_Interact(HitActor, OwnerPawn);

					if (bIsDebugging)
					{
						DebugSphereColor = FColor::Green;
						DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius, 12, DebugSphereColor, false, 3, 0, 2);
					}
					break;
				}
				if (bIsDebugging)
				{
					DebugSphereColor = FColor::Yellow;
					DrawDebugSphere(GetWorld(), hit.ImpactPoint, traceRadius, 12, DebugSphereColor, false, 3, 0, 2);
				}
			}
		}

		if (bIsDebugging)
		{
			FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
			DrawDebugLine(GetWorld(), EyeLocation, EndLocation, LineColor, false, 3, 0, 2);
		}
	}
}
