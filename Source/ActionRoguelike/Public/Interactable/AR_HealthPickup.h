// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AR_Pickup.h"
#include "GameFramework/Actor.h"
#include "AR_HealthPickup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AAR_HealthPickup : public AAR_Pickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAR_HealthPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool PickupBehavior(APawn* InstigatorPawn) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
