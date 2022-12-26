// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/AR_InteractionComponent.h"

#include "Blueprint/UserWidget.h"
#include "Interfaces/AR_IGameplayInterface.h"
#include "UserInterface/AR_WorldUserWidget.h"
#

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("ar_Debug.DrawInteraction"), false,TEXT("Enable drawing debug of the interaction action."),
	ECVF_Cheat);

// Sets default values for this component's properties
UAR_InteractionComponent::UAR_InteractionComponent(): TraceDistance(500), TraceRadius(30.0f),TraceCollisionChannel(ECC_WorldDynamic)
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

	FindBestInteractable();
}

void UAR_InteractionComponent::FindBestInteractable()
{
	bool bIsDebugging = CVarDebugDrawInteraction.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);

	FVector EndLocation;

	FVector EyeLocation;
	FRotator EyeRotation;
	AActor* Owner = GetOwner();
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	EndLocation = EyeLocation + EyeRotation.Vector() * TraceDistance;

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
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(TraceRadius);

		bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, EndLocation, FQuat::Identity,
		                                                       ObjectQueryParams,
		                                                       CollisionShape);
		// try to find best fitting actor to interact
		FocusedActor = nullptr;
		for (FHitResult hit : HitResults)
		{
			AActor* HitActor = hit.GetActor();
			if (HitActor)
			{
				FColor DebugSphereColor;
				if (bIsDebugging)
				{
					DebugSphereColor = FColor::Yellow;
					DrawDebugSphere(GetWorld(), hit.ImpactPoint, TraceRadius, 12, DebugSphereColor, false, 3, 0,
					                2);
				}
				if (HitActor->Implements<UAR_IGameplayInterface>())
				{
					// Only accepts if there is a clear line of sight to the interactable actor
					FHitResult VisibilityHit;
					GetWorld()->LineTraceSingleByChannel(VisibilityHit, EyeLocation, hit.ImpactPoint, ECC_Visibility);
					if (VisibilityHit.GetActor()) continue;

					FocusedActor = HitActor;

					if (bIsDebugging)
					{
						DebugSphereColor = FColor::Green;
						DrawDebugSphere(GetWorld(), hit.ImpactPoint, TraceRadius, 12, DebugSphereColor, false, 3,
						                0, 2);
					}
					break;
				}
			}
		}

		// Widget feedback for having or not a FocusedActor
		if (FocusedActor)
		{
			if (!InteractWidgetInstance && ensure(InteractWidgetClass))
			{
				InteractWidgetInstance = CreateWidget<UAR_WorldUserWidget>(GetWorld(), InteractWidgetClass);
			}

			if (InteractWidgetInstance)
			{
				InteractWidgetInstance->AttachedActor = FocusedActor;

				if (!InteractWidgetInstance->IsInViewport())
				{
					InteractWidgetInstance->AddToViewport();
				}
			}
		}
		else
		{
			if (InteractWidgetInstance && InteractWidgetInstance->IsInViewport())
			{
				InteractWidgetInstance->RemoveFromParent();
			}
		}


		// Debug Line for Interaction Attempt
		if (bIsDebugging)
		{
			FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
			DrawDebugLine(GetWorld(), EyeLocation, EndLocation, LineColor, false, 3, 0, 2);
		}
	}
}


void UAR_InteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "No focus actor to interact.");
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	IAR_IGameplayInterface::Execute_Interact(FocusedActor, OwnerPawn);
}
