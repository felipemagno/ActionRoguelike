// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/AR_HealthPickup.h"

#include "ActorComponent/AR_AttributeComponent.h"


// Sets default values
AAR_HealthPickup::AAR_HealthPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAR_HealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

bool AAR_HealthPickup::PickupBehavior(APawn* InstigatorPawn)
{
	Super::PickupBehavior(InstigatorPawn);

	UAR_AttributeComponent* Attribute = Cast<UAR_AttributeComponent>(
		InstigatorPawn->GetComponentByClass(UAR_AttributeComponent::StaticClass()));
	if (Attribute && !Attribute->IsFullHealth())
	{
		Attribute->ApplyHealthChange(this, 15);
		return true;
	}
	return false;
}

// Called every frame
void AAR_HealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
