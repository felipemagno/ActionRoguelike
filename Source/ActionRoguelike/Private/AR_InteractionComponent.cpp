// Fill out your copyright notice in the Description page of Project Settings.


#include "AR_InteractionComponent.h"


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
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector End;

	FVector EyeLocation;
	FRotator EyeRotation;
	AActor* Owner = GetOwner();
	Owner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	End =  EyeLocation + EyeRotation.Vector() * 1000;

	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
}
